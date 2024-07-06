/* FILE NAME  : def.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Definitions module.
 */

#ifndef __def_h_
#define __def_h_

#include "mth/mth.h"

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

typedef unsigned long long UINT64;
typedef long long INT64;

/* Project namespace */
namespace zart
{
  /* Rename structers */
  typedef mth::vec2<DBL> vec2;
  typedef mth::vec3<DBL> vec3;
  typedef mth::vec4<DBL> vec4;
  typedef mth::matr<DBL> matr;
  typedef mth::camera<DBL> camera;
  typedef mth::ray<DBL> ray;
} /* end of 'zart' namespace */

#endif /* __def_h_ */

/* END OF 'def.h' FILE */
