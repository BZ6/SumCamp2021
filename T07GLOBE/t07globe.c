/* FILE NAME: t07globe.c
 * PROGRAMMER: BZ6
 * DATE: 15.06.2021
 * PURPOSE: WinAPI application sample.
 */

#include <stdio.h>
#include "globe.h"

/* Window class name */
#define WND_CLASS_NAME "my window"

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
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG Msg;

  /* Fill window class structure */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return(0);
  }
  /* Window creation */
  hWnd =
    CreateWindow(WND_CLASS_NAME,
      "Title",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

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
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hDC;
  CHAR Buf[15];
  static BITMAP bm;
  static INT w, h;
  static HDC hDCFrame;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hDCFrame = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    GLB_TimerInit();
    SetTimer(hWnd, 47, 1, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hDCFrame, hBm);
    GlobeSet(w, h);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:  
    GLB_TimerResponse();
    SelectObject(hDCFrame, GetStockObject(WHITE_PEN));
    Rectangle(hDCFrame, 0, 0, w, h);
    SelectObject(hDCFrame, GetStockObject(DC_PEN));  
    GlobeDraw(hDCFrame, 300);
    SetBkMode(hDCFrame, TRANSPARENT);
    TextOut(hDCFrame, 5, 5, Buf, sprintf(Buf, "FPS: %.3f", GLB_FPS));
    hDC = GetDC(hWnd);
    BitBlt(hDC, 0, 0, w, h, hDCFrame, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hDCFrame);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_KEYDOWN:
    if (wParam == 'P')
      GLB_IsPause = !GLB_IsPause;
    return 0;

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 't07globe.c' FILE*/