/* FILE NAME  : winmsg.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 21.07.2021
 * PURPOSE    : Win message response module.
 */

#include "win.h"

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL zagl::win::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(hWnd, InitTimer, 1, NULL);
  return TRUE;
} /* End of 'win::OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID zagl::win::OnDestroy( VOID )
{
  if (IsInit)
  {
    Close();
    KillTimer(hWnd, RefreshTimer);
  }
  else
    KillTimer(hWnd, InitTimer);
} /* End of 'win::OnDestroy' function */


/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
VOID zagl::win::OnSize( UINT State, INT W, INT H )
{
  this->W = W;
  this->H = H;
  Resize();
} /* End of 'win::OnSize' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
BOOL zagl::win::OnEraseBkgnd( HDC hDC )
{
  return TRUE;
} /* End of 'win::OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window device context:
 *       HDC hDC;
 *   - paint message structure pointer:
 *       PAINTSTRUCT *PS;
 * RETURNS: None.
 */
VOID zagl::win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
} /* End of 'win::OnPaint' function */ 

/* WM_ACTIVATE window message handle function.
 * ARGUMENTS:
 *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
 *       UINT Reason;
 *   - handle of active window:
 *       HWND hWndActDeact;
 *   - minimized flag:
 *       BOOL IsMinimized;
 * RETURNS: None.
 */
VOID zagl::win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
{
  IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
} /* End of 'win::OnActivate' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID zagl::win::OnTimer( INT Id ) 
{
  if (!IsInit)
  {
    KillTimer(hWnd, InitTimer);
    SetTimer(hWnd, RefreshTimer, 1, NULL);
    IsInit = TRUE;
    // call callbacks
    Init();
    Resize();
    Timer();
  }
  else
    Timer();
} /* End of 'win::OnTimer' function */

/* WM_MOUSEWHEEL window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse wheel relative delta value:
 *       INT Z;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID zagl::win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  MouseWheel += Z;
} /* End of 'win::OnMouseWheel' function */

/* WM_*BUTTONDOWN windwow message hande function.
 * ARGUMENTS:
 *   - double click flag:
 *       BOOL IsDblClick;
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID zagl::win::OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys )
{
} /* End of 'win::OnButtonDown' function */

/* WM_*BUTTONUP windwow message hande function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID zagl::win::OnButtonUp( INT X, INT Y, UINT Keys )
{
} /* End of 'win::OnButtonUp' function */

VOID zagl::win::OnClose( VOID )
{
} /* End of 'zagl::win::OnClose' function */

VOID zagl::win::OnGetMinMaxInfo( MINMAXINFO *MinMax )
{
} /* End of 'zagl::win::OnGetMinMaxInfo' function */

/* END OF 'winmsg.h' FILE */
