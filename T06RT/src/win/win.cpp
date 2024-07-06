/* FILE NAME  : win.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Win module.
 */

#include "win.h"

/* win destructor */
zart::win::~win( VOID )
{
} /* End of 'zagl::win::~win' function */

/* win constructor */
zart::win::win( HINSTANCE hInst )
{
  hInstance = hInst;
  WNDCLASS wc;
  HWND hWnd;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(win *);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInst;
  wc.lpfnWndProc = win::WinFunc;
  wc.lpszClassName = "T06RT window class";

  /* Class register */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return;
  }

  /* Create window */
  hWnd = CreateWindow("T06RT window class", "3D animation",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));

  /* Show and update window*/
  ShowWindow(hWnd, SW_NORMAL);
  UpdateWindow(hWnd);
} /* End of 'zart::win::win' function */

/* Run function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID zart::win::Run( VOID )
{
  MSG msg;

  while (TRUE)
  {
    /* Check message at window message queue */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      if (msg.message == WM_QUIT)
        break;
      else
      {
        /* Displatch message to window */
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    else
      Idle();
  }
} /* End of win::Run function */

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
LRESULT CALLBACK zart::win::WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam )
{
  win *Win;
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetWindowLong(hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
  default:
    Win = reinterpret_cast<win *>(GetWindowLongPtr(hWnd, 0));
    if (Win != nullptr)
      switch (Msg)
      {
      case WM_CREATE:
        Win->hWnd = hWnd;
        return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
      case WM_SIZE:
        Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)Win->OnEraseBkgnd((HDC)wParam);
      case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Win->OnPaint(hDC, &ps);
        EndPaint(hWnd, &ps);
        return 0;
      case WM_TIMER:
        Win->OnTimer((UINT)wParam);
        return 0;
      case WM_MOUSEWHEEL:
        Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (INT)(SHORT)HIWORD(wParam),
                          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        SetCapture(hWnd);
        Win->OnButtonDown(FALSE,
                          (INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (UINT)(SHORT)LOWORD(wParam));
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        ReleaseCapture();
        Win->OnButtonUp((INT)(SHORT)LOWORD(lParam),
                        (INT)(SHORT)HIWORD(lParam),
                        (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_DESTROY:
        Win->OnDestroy();
        PostQuitMessage(30);
        return 0;
      }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* Flip full screen function.
 * ARGUMETNS: None.
 * RETURNS: None.
 */
VOID zart::win::FlipFullScreen( VOID )
{
  if (!IsFullScreen)
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    IsFullScreen = TRUE;

    /* Save old window size and position */
    GetWindowRect(hWnd, &FullScreenSaveRect);

    /* Get closest monitor */
    hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    /* Expand window to full screen */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_NOTOPMOST,
      FullScreenSaveRect.left, FullScreenSaveRect.top,
      FullScreenSaveRect.right - FullScreenSaveRect.left, FullScreenSaveRect.bottom - FullScreenSaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'zart::anim::FlipFullScreen' function */

/* END OF 'win.cpp' FILE */
