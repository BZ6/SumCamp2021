/* FILE NAME  : shader.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 25.07.2021
 * PURPOSE    : Shader class declaration.
 */

#ifndef __shader_h_
#define __shader_h_

#include <string>
#include <fstream>

#include "../../../zagl.h"
#include "resource.h"

namespace zagl
{
  class shader
  {
  private:
    /* Load text from file function.
     * ARGUMENTS:
     *   - file name to be load from:
     *       CHAR *FileName;
     * RETURNS:
     *   (CHAR *) load text (allocated through malloc).
     */
    static CHAR * LoadTextFromFile( CHAR *FileName )
    {
      std::fstream f(FileName, std::fstream::in);
      CHAR *txt;
      INT flen;

      /* Obtain file length */
      f.seekg(0, std::fstream::end);
      flen = f.tellg();
      f.seekg(0, std::fstream::beg);

      /* Allocate memory for file text */
      if ((txt = reinterpret_cast<CHAR *>(malloc(flen + 1))) == NULL)
        return NULL;

      /* Load text from file */
      memset(txt, 0, flen + 1);
      f.read(txt, flen);

      return txt;
    } /* End of 'LoadTextFromFile' function */

  public:
    INT Load( VOID )
    {
      struct
      {
        CHAR *Name; /* Shader file prefix name (e.g. "VERT") */
        INT Type;   /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
        INT Id;     /* Obtained shader Id from OpenGL */
      } shd[] =
      {
        {const_cast<CHAR *>("VERT"), GL_VERTEX_SHADER},
        {const_cast<CHAR *>("CTRL"), GL_TESS_CONTROL_SHADER},
        {const_cast<CHAR *>("EVAL"), GL_TESS_EVALUATION_SHADER},
        {const_cast<CHAR *>("GEOM"), GL_GEOMETRY_SHADER},
        {const_cast<CHAR *>("FRAG"), GL_FRAGMENT_SHADER},
      };
      INT NoofS = sizeof(shd) / sizeof(shd[0]), i, res, prg = 0;
      CHAR *txt;
      BOOL is_ok = TRUE;
      static CHAR Buf[1000];

      for (i = 0; i < NoofS; i++)
      {
        /* Build file name */
        sprintf_s(Buf, "BIN/SHADERS/%s/%s.GLSL", const_cast<CHAR *>(Name.c_str()), shd[i].Name);
        /* Load shader text from file */
        txt = LoadTextFromFile(Buf);
        if (txt == NULL)
        {
          if ((i == 0 || i == NoofS - 1))
          {
            ShaderLog(const_cast<CHAR *>(Name.c_str()), shd[i].Name, const_cast<CHAR *>("Error load file"));
            is_ok = FALSE;
            break;
          }
          continue;
        }
        /* Create shader */
        if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
        {
          if ((i == 0 || i == NoofS - 1))
          {
            ShaderLog(const_cast<CHAR *>(Name.c_str()), shd[i].Name, const_cast<CHAR *>("Error create shader"));
            free(txt);
            is_ok = FALSE;
            break;
          }
          continue;
        }
        /* Attach shader text to shader */
        glShaderSource(shd[i].Id, 1, &txt, NULL);
        free(txt);

        /* Compile shader */
        glCompileShader(shd[i].Id);
        glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
        if (res != 1)
        {
          glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
          ShaderLog(const_cast<CHAR *>(Name.c_str()), shd[i].Name, Buf);
          is_ok = FALSE;
          break;
        }
      }

      /* Create shader program */
      if (is_ok)
      {
        if ((prg = glCreateProgram()) == 0)
        {
          ShaderLog(const_cast<CHAR *>(Name.c_str()), const_cast<CHAR *>("PROG"), const_cast<CHAR *>("Error create program"));
          is_ok = FALSE;
        }
        else
        {
          /* Attach shaders to program */
          for (i = 0; i < NoofS; i++)
            if (shd[i].Id != 0)
              glAttachShader(prg, shd[i].Id);
          /* Link shader program */
          glLinkProgram(prg);
          glGetProgramiv(prg, GL_LINK_STATUS, &res);
          if (res != 1)
          {
            glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
            ShaderLog(const_cast<CHAR *>(Name.c_str()), const_cast<CHAR *>("PROG"), Buf);
            is_ok = FALSE;
          }
        }
      }

      if (!is_ok)
      {
        /* Delete all created shaders */
        for (i = 0; i < NoofS; i++)
          if (shd[i].Id != 0)
          {
            if (prg)
              glDetachShader(prg, shd[i].Id);
            glDeleteShader(shd[i].Id);
          }
        /* Delete program */
        if (prg)
          glDeleteProgram(prg);
        prg = 0;
      }
      return Prg = prg;
    } /* End of 'Load' function */
  private:
    /* Store log to file function.
     * ARGUMENTS:
     *   - message file prefix, shader name and text:
     *       CHAR *Prefix, *PartName, *Text;
     * RETURNS: None.
     */
    static VOID ShaderLog( CHAR *Prefix, CHAR *PartName, CHAR *Text )
    {
      std::fstream f("BIN/SHADERS/SHD.LOG", std::fstream::out);

      f << Prefix << "/" << PartName << ".glsl" << "\n" << Text << "\n";
    } /* End of 'ShaderLog' function */

  public:
    std::string Name;
    INT Prg;

    shader( VOID ): Prg(0)
    {
    }

    /* Shader class constructor */
    shader( CHAR *ShdFileNamePrefix )
    {
      Name = ShdFileNamePrefix;
      Prg = Load();
    } /* End of 'shader' function */

    /* Unload shader program from memory files function.
     * ARGUMENTS:
     *   - shader program Id:
     *       INT ProgId;
     * RETURNS: None.
     */
    VOID Update( VOID )
    {
      UINT shdrs[5];
      INT n;

      if (Prg == 0)
        return;

      glGetAttachedShaders(Prg, 5, &n, shdrs);
      for (INT i = 0; i < n; i++)
      {
        glDetachShader(Prg, shdrs[i]);
        glDeleteShader(shdrs[i]);
      }
      glDeleteProgram(Prg);

      *this = shader(const_cast<CHAR *>(Name.c_str()));
    } /* End of 'Update' function */

    VOID Free( VOID )
    {
    }

  }; /* End of 'shader' class */

  /* Shader class */
  class shader_manager : public resource_manager<shader, std::string>
  {
  public:
    /* Shader uniform value set function.
     * ARGUMENTS:
     *   - uniform name:
     *       const CHAR *Name;
     *   - uniform value:
     *       const value_type &Value;
     * RETURNS: None.
     */
    template<typename value_type>
      VOID UniSet( const CHAR *Name, const value_type &Value )
      {
        INT loc, prg = resource_manager::Find("DEFAULT")->Prg;
 
        glUseProgram(prg);
        if ((loc = glGetUniformLocation(prg, Name)) != -1)
          if constexpr (std::is_convertible_v<FLT, value_type>)
            glUniform1f(loc, Value);
          else if constexpr (std::is_convertible_v<INT, value_type>)
            glUniform1i(loc, Value);
          else if constexpr (std::is_convertible_v<vec3, value_type>)
            glUniform3fv(loc, 1, Value);
          else if constexpr (std::is_convertible_v<matr, value_type>)
            glUniformMatrix4fv(loc, 1, FALSE, Value);
      }

    /* Shader manager constructor */
    shader_manager( VOID ) : resource_manager<shader, std::string>()
    {
    } /* End of 'shader_manager' function */

    /* Add shader function.
     * ARGUMENTS:
     *   - shader name:
     *       CHAR *ShdFileNamePrefix;
     * RETURNS: None.
     */
    VOID Add( CHAR *ShdFileNamePrefix )
    {
      resource_manager::add(shader(ShdFileNamePrefix));
    } /* End of 'Add' function */

    VOID Update( VOID )
    {
      resource_manager::Update();
      resource_manager::Init();
    } /* End of 'ShaderUpdate' function */
  };
} /* end of 'zagl' namespace */

#endif __shader_h_

/* END OF 'shader.h' FILE */
