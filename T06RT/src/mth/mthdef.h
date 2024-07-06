/* FILE NAME  : mthdef.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Math definitions module.
 */


#ifndef WIN32
  #define WIN32
  #include <commondf.h>
  #undef WIN32
#else
  #include <commondf.h>
#endif /* WIN32 */

typedef DOUBLE DBL;
typedef FLOAT FLT;

namespace mth
{
  /* 3D vector representation type */
  template<class Type>
    class vec3;
  /* 2D vector representation type */
  template<class Type>
    class vec2;
  /* 4D vector representation type */
  template<class Type>
    class vec4;
  /* Matrix representation type */
  template<class Type>
    class matr;
  /* Ray vector representation type */
  template<class Type>
    class ray;
  /* Camera representation type */
  template<class Type>
    class camera;
}

#define PI 3.14159265358979323846

/* END OF 'mthdef.h' FILE */
