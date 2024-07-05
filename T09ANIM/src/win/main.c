/* FILE NAME: main.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D main module.
 */

#include "../game/game.h"

/* Window class name */
#define BZ6_WND_CLASS_NAME "My Window Class Name"

/* Forward declaration */
LRESULT CALLBACK BZ6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG Msg;
  WNDCLASS wc;

  SetDbgMemHooks();

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = BZ6_WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = BZ6_WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error regoister window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd =
    CreateWindow(BZ6_WND_CLASS_NAME,
      "Title",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  ShowWindow(hWnd, CmdShow);
  UpdateWindow(hWnd);

  BZ6_AnimAddUnit(BZ6_UnitCreateCtrl());
  BZ6_AnimAddUnit(BZ6_UnitCreatePlane());
  BZ6_AnimAddUnit(BZ6_UnitCreateTorus());
  BZ6_AnimAddUnit(BZ6_UnitCreateFence());
  BZ6_AnimAddUnit(BZ6_UnitCreateCow());
  BZ6_AnimAddUnit(BZ6_UnitCreateCowRandom());
  //BZ6_AnimAddUnit(BZ6_UnitCreateTex());

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
    {
      if (Msg.message == WM_QUIT)
        break;
      DispatchMessage(&Msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);

  return 30;
} /* End of 'WinMain' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK BZ6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_CREATE:
    GameInit(hWnd);
    SetTimer(hWnd, 30, 1, NULL);
    return 0;

  case WM_SIZE:
    GameResize(LOWORD(lParam), HIWORD(lParam));
    
    /* Redraw frame */
    SendMessage(hWnd, WM_TIMER, 30, 0);
    return 0;

  case WM_TIMER:
    GameRender();
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_PAINT:
    BeginPaint(hWnd, &ps);
    GameCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_KEYDOWN:
    if (wParam == 27)
      BZ6_AnimExit();
    return 0;

  case WM_MOUSEWHEEL:
    BZ6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_DESTROY:
    GameClose();
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'BZ6_WinFunc' function */

 /* END OF 'main.c' FILE */