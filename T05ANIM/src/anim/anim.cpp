/* FILE NAME  : anim.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Animation class functoions definitions.
 */

#include <cstdio>

#include "anim.h"

zagl::anim zagl::anim::Instance;

/* Init callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Init( VOID )
{
} /* End of 'zagl::anim::Init' function */

/* Close callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Close( VOID )
{
} /* End of 'zagl::anim::Close' function */

/* Resize callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Resize( VOID )
{
  render::Resize();
} /* End of 'zagl::anim::Resize' function */

/* Timer callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Timer( VOID )
{
  CHAR Buf[100];

  sprintf_s(Buf, "%.0f", FPS);
  SetWindowTextA(win::hWnd, Buf);
  Render();
  CopyFrame();
} /* End of 'zagl::anim::Timer' function */

/* Activate callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Activate( VOID )
{
} /* End of 'zagl::anim::Activate' function */

/* Idle callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Idle( VOID )
{
  Render();
  CopyFrame();
} /* End of 'zagl::anim::Idle' function */

/* Erase callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Erase( HDC hDC )
{
} /* End of 'zagl::anim::Erase' function */

/* Paint callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::anim::Paint( HDC hDC )
{
} /* End of 'zagl::anim::Paint' function */

//zagl::scene & zagl::scene::operator<<( const std::string &Name )
//{
//  zagl::anim &Ani = zagl::anim::Get();
//  if (Ani.UnitNames.find(Name) != Ani.UnitNames.end())
//    return *this << Ani.UnitNames[Name]();
//  return *this;
//}

/* END OF 'anim.cpp' FILE */
