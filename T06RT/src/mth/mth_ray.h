/* FILE NAME  : mth_ray.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Math vector module.
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include <cstdlib>
#include <cmath>

#include "mth_vec3.h"

 /* Space math namespace */
  namespace mth
{
  /* ray vector representation type */
  template<class Type>
    class ray
    {
    public:
      vec3<Type>
        Org,
        Dir;

      /* Default constructor */
      ray( VOID )
      {
      } /* End of 'ray' constructor */

      /* Class constructor
       * ARGUMENTS:
       *   - ray origin:
       *       const vec3<Type> &Origin;
       *   - ray direction:
       *       const vec3<Type> &Direction.
       * RETURNS: None.
       */
      ray( const vec3<Type> &Origin, const vec3<Type> &Direction ) :
        Org(Origin), Dir(Direction.Normalizing())
      {
      } /* End of 'ray' constructor */

      /* Obtain ray point function
       * ARGUMENTS:
       *   - ray point parameter:
       *       Type T;
       * RETURNS: (vec3<Type>) ray point. 
       */
      vec3<Type> operator()( Type T ) const
      {
        return Org + Dir * T;
      } /* End of 'operator()' function */

    }; /* End of 'ray' class */
} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */