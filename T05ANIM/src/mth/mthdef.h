/* FILE NAME  : mthdef.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 21.07.2021
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

#define PI 3.14159265358979323846

/* END OF 'mthdef.h' FILE */
