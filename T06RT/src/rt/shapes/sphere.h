/* FILE NAME  : sphere.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 03.08.2021
 * PURPOSE    : Shape sphere module.
 */

#ifndef __sphere_h_
#define __sphere_h_

#include "../rt.h"

namespace zart
{
  /* Shape sphere class */
  class sphere : public shape
  {
  public:
    DBL r2; // Square of radius of sphere
    vec3 C; // Location of center of sphere

    /* Default constructor */
    sphere( VOID ) : r2(1), C(0)
    {
    } /* End of 'sphere' constructor */

    /* Create sphere constructor.
     * ARGUMENTS:
     *   - location of center of sphere:
     *       vec3<DBL> &L;
     *   - radius of sphere:
     *       DBL r.
     */
    sphere( const vec3 &L, DBL r ) : r2(r *r), C(L.X, L.Y, L.Z)
    {
    } /* End of 'sphere' constructor */

    /* Count intersection sphere function.
     * ARGUMENTS:
     *   - ray of view:
     *       const ray<DBL> &R;
     *   - place for member intersection:
     *       intr *Intr.
     * RENURNS: (BOOL) if succed TRUE else FALSE.
     */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      vec3 a = C - R.Org;
      DBL OK = a & R.Dir, h2 = r2 - a.Length2() + OK * OK;

      if (a.Length2() < r2)
      {
        Intr->T = OK + sqrt(h2);
        Intr->N = (R(Intr->T) - C).Normalizing();
        return TRUE;
      }
      if (OK < -Trashhold)
        return FALSE;
      if (h2 < -Trashhold)
        return FALSE;

      Intr->T = OK - sqrt(h2);
      Intr->N = (R(Intr->T) - C).Normalizing();
      return TRUE;
    } /* End of 'Intersection' function */
  }; /* End of 'sphere' class */
} /* end of 'zart' namespace */

#endif /* __sphere_h_ */

/* END OF 'sphere.h' FILE */