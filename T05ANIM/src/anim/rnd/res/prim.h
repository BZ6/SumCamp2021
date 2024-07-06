/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
/* FILE NAME   : prim.h
 * PROGRAMMER  : Bogdan Zakharkin.
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Primitive structure.
 */

#ifndef __prim_h_
#define __prim_h_

#include "topology.h"
#include "materials.h"

/* NSF name space */
namespace zagl
{
  /* Primitive handle class */
  class prim
  {
    friend class render;
  private:
    prim_type Type;    /* Primitive type */
    UINT VA;           /* Primitive vertex array */
    UINT VBuf;         /* Vertex buffer */
    UINT IBuf;         /* Index buffer */
    INT NumOfElements; /* Number of elements for OpenGL */
 
  public:
    matr Transform;    /* Primitive transformation matrix */
    vec3
      MinBB, /* Minimal primitive position */
      MaxBB; /* Maximal primitive position */
    material *Mtl;     /* Material pointer */
    INT MtlNo;     /* Material pointer */
 
    /* Class constructor */
    prim( VOID ) : VA(0), VBuf(0), IBuf(0), NumOfElements(0), Transform(matr::Identity()), MinBB(0), MaxBB(0), Type(zagl::prim_type::TRIMESH)
    {
    } /* End of 'prim' function */
 
    /* Class destructor */
    ~prim( VOID )
    {
    } /* End of '~prim' function */

    /* Free render primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      if (VA != 0)
      {
        /* making an array of vertices active */
        glBindVertexArray(VA);
        /* unhooking the buffer */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBuf);
        /* making an array of vertices inactive */
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VA);

        /* making an array of indexes inactive */
        glDeleteBuffers(1, &IBuf);
        VA = 0, VBuf = 0, IBuf = 0;
      }
      else
        VBuf = 0, IBuf = 0;
    } /* End of 'Free' function */
 
    


    /* Primitive creation function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     * RETURNS:
     *   (prim &) self reference.
     */
    template<typename vertex>
      prim & Create( const topology::base<vertex> &T )
      {
        Free();
        Type = T.Type;
        // Setup data order due to vertex description string
        const std::string dsc = vertex::Description;

        if (!T.V.empty() && T.V.size() != 0)
        {
          glGenBuffers(1, &VBuf);
          glGenVertexArrays(1, &VA);
 
          glBindVertexArray(VA);
          glBindBuffer(GL_ARRAY_BUFFER, VBuf);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * T.V.size(), T.V.data(), GL_STATIC_DRAW);
 
          INT attr = 0, offset = 0;
          for (auto c = dsc.begin(); c != dsc.end(); c++)
            if (*c == 'f' || *c == 'i')
            {
              CHAR type = *c++;
              INT n = *c - '0';
 
              if (n < 0 || n > 9)
                break;
              // Enable used attributes
              glEnableVertexAttribArray(attr);
              glVertexAttribPointer(attr, n, type == 'f' ? GL_FLOAT : GL_INT, FALSE,
                sizeof(vertex), reinterpret_cast<VOID *>(offset));
              offset += n * 4;
              attr++;
            }
            else
              break;
          glBindVertexArray(0);
        }
        if (!T.I.empty() && T.I.size() != 0)
        {
          glGenBuffers(1, &IBuf);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuf);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * T.I.size(), T.I.data(), GL_STATIC_DRAW);
          NumOfElements = static_cast<INT>(T.I.size());
        }
        else
          NumOfElements = static_cast<INT>(T.V.size());
        Transform = matr::Identity();
        return *this;
      } /* End of 'Create' function */
  }; /* End of 'prim' class */
} /* end of 'zagl' namespace */

#endif /* __prim_h_ */

/* END OF 'prim.h' FILE */