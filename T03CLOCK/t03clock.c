/* FILE NAME: pattern.c
 * PROGRAMMER: BZ6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI application sample.
 */

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#pragma warning(disable: 4244)

/* Window class name */
#define WND_CLASS_NAME "my window"

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam);
VOID DrawHand( HDC hDC, INT x, INT y, INT l, INT w, DOUBLE a );

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
  while (GetMessage(&Msg, NULL, 0, 0))
  {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
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
  SYSTEMTIME st;
  HFONT hFnt;
  HPEN hPen;
  POINT pt;
  INT i;
  CHAR Buf[11];
  DOUBLE a, pi = 3.14159265358979323846;
  static BITMAP bm, bmIc;
  static INT w, h, r;
  static HDC hDCFrame, hDCLogo, hDCIcon;
  static HBITMAP hBm, hBmLogo, hBmAnd, hBmXor;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hDCFrame = CreateCompatibleDC(hDC);
    hDCLogo = CreateCompatibleDC(hDC);
    hDCIcon = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBmLogo = LoadImage(NULL, "clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  
    hBmAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hDCLogo, hBmLogo);
    GetObject(hBmLogo, sizeof(BITMAP), &bm);
    GetObject(hBmAnd, sizeof(BITMAP), &bmIc);
    SetTimer(hWnd, 47, 10, NULL);
    return 0;

  case WM_TIMER:  
    GetLocalTime(&st);
    SelectObject(hDCFrame, GetStockObject(WHITE_PEN));
    Rectangle(hDCFrame, 0, 0, w, h);
    SelectObject(hDCFrame, GetStockObject(DC_PEN));
    SetStretchBltMode(hDCFrame, COLORONCOLOR);
    r = w > h ? h : w;
    StretchBlt(hDCFrame, (w - r) / 2, (h - r) / 2, r, r, hDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    hPen = CreatePen(PS_SOLID, r / 100, RGB(255, 0, 0));
    SelectObject(hDCFrame, hPen);
    SelectObject(hDCFrame, GetStockObject(NULL_BRUSH));
    a = (st.wSecond + st.wMilliseconds / 1000.0) * 2 * pi / 60;
    DrawHand(hDCFrame,  w / 2, h / 2, r / 3, r / 100, a); 
    DeleteObject(hPen);
    hPen = CreatePen(PS_SOLID, r / 75, RGB(0, 255, 0));
    SelectObject(hDCFrame, hPen);
    a = (st.wMinute + st.wSecond / 60.0) * 2 * pi / 60;
    DrawHand(hDCFrame,  w / 2, h / 2, r / 6, r / 75, a);
    DeleteObject(hPen);
    hPen = CreatePen(PS_SOLID, r / 50, RGB(0, 0, 255));
    SelectObject(hDCFrame, hPen); 
    a = (st.wHour % 12 + st.wMinute / 60.0) * 2 * pi / 12;
    DrawHand(hDCFrame,  w / 2, h / 2, r / 9, r / 50, a);
    hFnt = CreateFont(211, 0, 50, 0, FW_BOLD,
             FALSE, FALSE, FALSE, RUSSIAN_CHARSET, 
             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
             PROOF_QUALITY, FIXED_PITCH | FF_ROMAN, 
             "Times");        
    TextOut(hDCFrame, 0, 0, Buf, sprintf(Buf, "%02d.%02d.%02d", st.wDay, st.wMonth, st.wYear));
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt); 
    SelectObject(hDCIcon, hBmAnd);
    BitBlt(hDCFrame, pt.x - bmIc.bmWidth / 2, pt.y - bmIc.bmHeight / 2, bmIc.bmWidth, bmIc.bmHeight, hDCIcon, 0, 0, SRCAND);
    SelectObject(hDCIcon, hBmXor);
    BitBlt(hDCFrame, pt.x - bmIc.bmWidth / 2, pt.y - bmIc.bmHeight / 2, bmIc.bmWidth, bmIc.bmHeight, hDCIcon, 0, 0, SRCINVERT); 
    pt.x = w * (0.5 + 0.4 * sin(2 * clock() / 1000.0)); 
    pt.y = h * (0.5 + 0.4 * cos(clock() / 1000.0)); 
    for (i = 0; i < 63; i++)
    {
      pt.x = w * (0.5 + 0.4 * sin(2 * clock() / 1000.0 + i / 10.0)); 
      pt.y = h * (0.5 + 0.4 * cos(clock() / 1000.0 + i / 10.0));

      SelectObject(hDCIcon, hBmAnd);
      BitBlt(hDCFrame, pt.x - bmIc.bmWidth / 2, pt.y - bmIc.bmHeight / 2, bmIc.bmWidth, bmIc.bmHeight, hDCIcon, 0, 0, SRCAND);
      SelectObject(hDCIcon, hBmXor);
      BitBlt(hDCFrame, pt.x - bmIc.bmWidth / 2, pt.y - bmIc.bmHeight / 2, bmIc.bmWidth, bmIc.bmHeight, hDCIcon, 0, 0, SRCINVERT);
    }
    SelectObject(hDCFrame, GetStockObject(DC_PEN));
    SelectObject(hDCFrame, GetStockObject(DC_BRUSH));
    DeleteObject(hFnt);
    DeleteObject(hPen);  
    InvalidateRect(hWnd, NULL, FALSE);
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
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmLogo);
    DeleteObject(hBmAnd);
    DeleteObject(hBmXor);
    DeleteDC(hDCFrame);
    DeleteDC(hDCLogo);
    DeleteDC(hDCIcon);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;

  case WM_ERASEBKGND:
    return 1; 
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle descriptor:
 *      HDC hDC;
 *   - coordinetes of center:
 *      INT x, y;
 *   - size of hand:
 *      INT l, w;
 *   - anle in radians:
 *      DOUBLE a;
 * RETURNS:
 *   VOID.
 */
VOID DrawHand( HDC hDC, INT x, INT y, INT l, INT w, DOUBLE a )
{
  POINT pnts[4];
  DOUBLE s = sin(a), c = cos(a);

  pnts[0].x = x + l * s;
  pnts[0].y = y - l * c;
  pnts[1].x = x - w * c;
  pnts[1].y = y - w * s;
  pnts[2].x = x - w * s;
  pnts[2].y = y + w * c;
  pnts[3].x = x + w * c;
  pnts[3].y = y + w * s;

  Polygon(hDC, pnts, 4);
} /* End of 'DrawHand' function */

/* END OF 't03clock.c' FILE*/