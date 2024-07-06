/* FILE NAME   : materials.h
 * PROGRAMMER  : BZ6
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Materials class declaration.
 */

#ifndef __materials_h_
#define __materials_h_

#include "../../../zagl.h"

/* NSF name space */
namespace zagl
{
  class shader_manager;

  /* Material class */
  class material
  {
  public:
    /* Illumination coefficients */
    vec3
      Ka,                   /* Ambient coefficient */
      Kd,                   /* Diffuse coefficient */
      Ks;                   /* Specular coefficient */
    FLT Ph;                 /* Phong power coefficient */
    FLT Trans;              /* Transparency factor */
    INT Tex[8];     /* Texture references from texture table (or -1) */
    INT 
      KaLoc, /* Uniform buffer locations */
      KdLoc,
      KsLoc,
      PhLoc,
      TexLoc[8];

    std::string ShdName;
    std::string Name;

    /* Material constructor */
    material( VOID ) : Ka(vec3(0.25, 0.148, 0.06475)), Kd(vec3(0.4, 0.2368, 0.1036)), Ks(vec3(0.774597, 0.458561, 0.200621)), Ph(10), Trans(1)
    {
      for (INT i = 0; i < 8; i++)
        Tex[i] = -1;
      ShdName = "DEFAULT";
      Name = "DefaultMtl";
    } /* End of 'material' function */

    VOID Free( VOID )
    {
    }
  }; /* End of 'material' class */

  /* Material manager class */
  class material_manager : public resource_manager<material, std::string>
  {
  public:
    /* Shader manager constructor */
    material_manager( VOID ) : resource_manager<material, std::string>()
    {
    } /* End of 'shader_manager' function */

    /* Add shader function.
     * ARGUMENTS:
     *   - shader name:
     *       CHAR *ShdFileNamePrefix;
     * RETURNS: None.
     */
    material * Add( material &Mtl )
    {
      return resource_manager::add(Mtl);
    } /* End of 'Add' function */

    VOID MtlApply( INT MtlNo )
    {
      //material::Apply();
    }
  }; /* End of 'material_manager' class */
} /* end of 'zagl' namespace */

#endif /* __materials_h_ */

/* END OF 'materials.h' FILE */
