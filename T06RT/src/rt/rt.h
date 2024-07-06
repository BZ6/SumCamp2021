/* FILE NAME  : rt.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 03.08.2021
 * PURPOSE    : Ray tracing definition module.
 */

#ifndef __rt_h_
#define __rt_h_

#include <vector>

#include "../def.h"

/* Project namespace */
namespace zart
{
  DBL Trashhold = 0.0001; // Calculation error
 
  /* Member for intersection */
  class intr
  {
  public:
    DBL T;    // T of intesection of ray
    vec3 N;   // Normal
    BOOL IsN; // Flag of count normal
    vec3 P;   // Position of intesection of ray
    BOOL IsP; // Flag of count position of intesection of ray
  }; /* End of 'intr' class */

  /* Member for material of object */
  class surface
  {
    vec3 Ka, Kd, Ks; // ambient, diffuse, specular
    DBL Ph;          // Bui Tong Phong coefficient
    DBL Kr, Kt;      // reflected, transmitted
  }; /* End of 'surface' class */

  /* Information of light */
  class light_info
  {
    vec3 L;     // Light source direction
    vec3 Color; // Light source color
    DBL Dist;   // Distance to light source
  }; /* End of 'light_info' class */

  /* Member fot calculated light */
  class light
  {
    DBL Cc, Cl, Cq;

    /* Calculate shadow funcion.
     * ARGUMENTS:
     *   - position of intersection:
     *       const vec3 P;
     *   - information of light:
     *       light_info *L.
     * RETURNS: (DBL) attetuation.
     */
    virtual DBL Shadow( const vec3 &P, light_info *L )
    {
    } /* End of 'Shadow' function */
  }; /* End of 'light' class */
  /* Base shape class */
  class shape
  {
  public:
    /* Count intersection base function.
     * ARGUMENTS:
     *   - ray of view:
     *       const ray<DBL> &R;
     *   - place for member intersection:
     *       intr *Intr.
     * RENURNS: (BOOL) if succed TRUE else FALSE.
     */
    virtual BOOL Intersection( const ray &R, intr *Intr )
    {
      return FALSE;
    } /* End of 'Intersection' function */

    /* Get normal base function.
     * ARGUMENTS:
     *   - place for member intersection:
     *       intr *Intr.
     * RENURNS: None.
     */
    virtual VOID GetNormal( intr *I )
    {
    }
  }; /* End of 'GetNormal' class */

  /* Scene class */
  class scene
  {
  public:
    std::vector<shape *> Shapes; //Stock of usising shapes

    /* Count intersection function.
     * ARGUMENTS:
     *   - ray of view:
     *       const ray<DBL> &R;
     *   - place for member intersection:
     *       intr *Intr.
     * RENURNS: (BOOL) if succed TRUE else FALSE.
     */
    BOOL Intersection( const ray &R, intr *Intr )
    {
      intr in, cn;
      INT k = 0;

      if (Shapes.size() == 0)
        return FALSE;
      for (INT i = 0; i < Shapes.size(); i++)
        if (Shapes[i]->Intersection(R, &in) && (in.T < cn.T || k == 0))
            cn = in, k = 1;
      if (k == 0)
        return FALSE;
      *Intr = cn;
      return TRUE;
    } /* End of 'Intersection' function */

    /* Add shape for scene function */
    scene & operator<<( shape *Sh )
    {
      Shapes.push_back(Sh);
      return *this;
    } /* End of 'operator<<' function */

    /* Default destructor*/
    ~scene(VOID)
    {
      for (INT i = 0; i < Shapes.size(); i++)
        delete Shapes[i];
      Shapes.clear();
    } /* End of 'scene' distructor */
  }; /* End of 'scene' class */
} /* end of 'zart' namespace */

#endif /* __rt_h_ */

/* END OF 'rt.h' FILE */