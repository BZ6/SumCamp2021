/* FILE NAME: anim.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation function module.
 */

#include "anim.h"

bz6ANIM BZ6_Anim;

/* Animation initialization function.
 * ARGUMENTS:
 *   - handle window:
 *       HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimInit( HWND hWnd )
{
  srand(30);
  BZ6_Anim.hWnd = hWnd;
  BZ6_RndInit(hWnd);
  BZ6_TimerInit();
  BZ6_AnimInputInit();
} /* End of 'BZ6_AnimInit' function */

/* Animation close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < BZ6_Anim.NumOfUnits; i++)
  {
    BZ6_Anim.Units[i]->Close(BZ6_Anim.Units[i], &BZ6_Anim);
    free(BZ6_Anim.Units[i]);
    BZ6_Anim.Units[i] = NULL;
  }
  BZ6_Anim.NumOfUnits = 0;
  BZ6_RndClose();
} /* End of 'BZ6_AnimClose' function */

/* Animation unit delete function.
 * ARGUMENTS:
 *   NONE.
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimDelUnit( VOID )
{
  BZ6_Anim.NumOfUnits--;
  BZ6_Anim.Units[BZ6_Anim.NumOfUnits]->Close(BZ6_Anim.Units[BZ6_Anim.NumOfUnits], &BZ6_Anim);
  free(BZ6_Anim.Units[BZ6_Anim.NumOfUnits]);
  BZ6_Anim.Units[BZ6_Anim.NumOfUnits] = NULL;
} /* End of 'BZ6_AnimDelUnit' function */

/* Animation resize function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, H;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimResize( INT W, INT H )
{
  BZ6_Anim.W = W;
  BZ6_Anim.H = H;
  BZ6_RndResize(W, H);
} /* End of 'BZ6_AnimResize' function */

/* Animation copy frame function.
 * ARGUMENTS:
 *   - handle descriptor:
 *       HDC hDC;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimCopyFrame( VOID )
{
  BZ6_RndCopyFrame();
} /* End of 'BZ6_AnimCopyFrame' function */

/* Animation render function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimRender( VOID )
{
  INT i;

  BZ6_TimerResponse();
  BZ6_AnimInputResponse();

  for (i = 0; i < BZ6_Anim.NumOfUnits; i++)
    BZ6_Anim.Units[i]->Response(BZ6_Anim.Units[i], &BZ6_Anim);

  BZ6_RndStart();
  for (i = 0; i < BZ6_Anim.NumOfUnits; i++)
    BZ6_Anim.Units[i]->Render(BZ6_Anim.Units[i], &BZ6_Anim);
  BZ6_RndEnd();
} /* End of 'BZ6_AnimRender' function */

/* Unit add in animation function.
 * ARGUMENTS:
 *   - pointer to created unit:
 *       bz6UNIT *Uni;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimAddUnit( bz6UNIT *Uni )
{
  if (BZ6_Anim.NumOfUnits < BZ6_MAX_UNITS)
    BZ6_Anim.Units[BZ6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &BZ6_Anim);
} /* End of 'BZ6_AnimAddUnit' function */

/* Toggle window fullscreen mode function.
 * ARGUMENTS: NONE;
 * RETURNS: None.
 */
VOID BZ6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    IsFullScreen = TRUE;

    /* Goto fullscreen mode */

    /* Store current window size and position */
    GetWindowRect(BZ6_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(BZ6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(BZ6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(BZ6_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(BZ6_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'BZ6_AnimFlipFullScreen' function */

/* Toggle window axit mode function.
 * ARGUMENTS: NONE;
 * RETURNS: None.
 */
VOID BZ6_AnimExit( VOID )
{
  SendMessage(BZ6_Anim.hWnd, WM_DESTROY, 0, 0);
} /* End of 'BZ6_AnimExit' function */

/* END OF 'anim.c' FILE */