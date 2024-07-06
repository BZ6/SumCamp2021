/* FILE NAME  : mth.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Math module.
 */

#include "mth_vec2.h"
#include "mth_vec3.h"
#include "mth_vec4.h"
#include "mth_matr.h"
#include "mth_camera.h"
#include "mth_ray.h"

namespace mth
{
  /* Minimum of 2 values.
   * ARGUMENTS:
   *   - values for comparing:
   *       type A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
    static type Min( type A, type B )
    {
      return A < B ? A : B;
    } /* End of 'Min' function */

  /* Minimum of 2 vectors.
   * ARGUMENTS:
   *   - vectors for comparing:
   *       vec3<type> A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
    static vec3<type> MinBB( vec3<type> A, vec3<type> B )
    {
      return vec3<type>(Min(A.X, B.X), Min(A.Y, B.Y), Min(A.Z, B.Z));
    } /* End of 'MinBB' function */

  /* Maximum of 2 values.
   * ARGUMENTS:
   *   - values for comparing:
   *       type A, B;
   * RETURNS:
   *   (type) maximal value.
   */
  template<typename type>
    static type Max( type A, type B )
    {
      return A > B ? A : B;
    } /* End of 'Max' function */

  /* Maximum of 2 vectors.
   * ARGUMENTS:
   *   - vectors for comparing:
   *       vec3<type> A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
    static vec3<type> MaxBB( vec3<type> A, vec3<type> B )
    {
      return vec3<type>(Max(A.X, B.X), Max(A.Y, B.Y), Max(A.Z, B.Z));
    } /* End of 'MaxBB' function */
}

/* END OF 'mth.h' FILE */
