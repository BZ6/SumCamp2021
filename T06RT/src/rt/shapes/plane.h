/* FILE NAME  : plane.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 03.08.2021
 * PURPOSE    : Shape plane module.
 */

#ifndef __plane_h_
#define __plane_h_

#include "../rt.h"

namespace zart
{
  /* Shape plane class */
  class plane : public shape
  {
  public:
    vec3 N;  // Normal of plane
    DBL Dis; // Distance of normal

    /* Default constructor */
    plane( VOID ) : N(0, 1, 0), Dis(0)
    {
    } /* End of 'plane' constructor */

    /* Create plane constructor.
     * ARGUMENTS:
     *   - normal of plane:
     *       vec3<DBL> &N;
     *   - distance of normal:
     *       DBL Dis.
     */
    plane( const vec3 &N, DBL Dis ) : N(N.X, N.Y, N.Z), Dis(Dis)
    {
    } /* End of 'plane' constructor */

    /* Count intersection plane function.
     * ARGUMENTS:
     *   - ray of view:
     *       const ray<DBL> &R;
     *   - place for member intersection:
     *       intr *Intr.
     * RENURNS: (BOOL) if succed TRUE else FALSE.
     */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      if (-(N & R.Dir) < Trashhold)
        return FALSE;

      Intr->T = -((N & R.Org) + Dis) / (N & R.Dir);
      Intr->N = N;
      return TRUE;
    } /* End of 'Intersection' function */
  }; /* End of 'plane' class */
} /* end of 'zart' namespace */

#endif /* __plane_h_ */

/* END OF 'plane.h' FILE */