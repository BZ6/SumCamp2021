/* t02eyes.c
 */
#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "my window"


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam);

VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG Msg;

  /* */
  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /**/
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return(0);
  }
  /**/
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
  /**/
  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  while (GetMessage(&Msg, NULL, 0, 0))
  {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return 30;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hDC;
  HPEN hPen;
  POINT pt;
  static INT w, h;
  static HDC hDCFrame;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hDCFrame = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetTimer(hWnd, 47, 10, NULL);
    return 0;
  case WM_TIMER:  
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    SelectObject(hDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hDCFrame, hPen);
    Rectangle(hDCFrame, 0, 0, w, h);
    DrawEye(hDCFrame, w / 4, h / 4, 100, 30, pt.x, pt.y);
    DrawEye(hDCFrame, 3 * w / 4, 3 * h / 4, 100, 30, pt.x, pt.y);
    DrawEye(hDCFrame, 3 * w / 4, h / 4, 100, 30, pt.x, pt.y);
    DrawEye(hDCFrame, w / 4, 3 * h / 4, 100, 30, pt.x, pt.y);
    SelectObject(hDCFrame, GetStockObject(DC_PEN));
    SelectObject(hDCFrame, GetStockObject(DC_BRUSH));
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
    DeleteDC(hDCFrame);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  case WM_ERASEBKGND:
    return 1; 
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  INT dx, dy;
  DOUBLE kf, len = sqrt((Mx - X) * (Mx - X) + (My - Y) * (My - Y));
  HPEN hPen;
  HBRUSH hBr;

  hPen = CreatePen(PS_SOLID, 5, RGB(1, 1, 1));
  SelectObject(hDC, hPen);
  hBr = CreateSolidBrush(RGB(200, 200, 200));
  SelectObject(hDC, hBr);
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  hPen = CreatePen(PS_SOLID, 2, RGB(1, 1, 1));
  SelectObject(hDC, hPen);
  hBr = CreateSolidBrush(RGB(0, 20, 0));
  SelectObject(hDC, hBr);
  if (R - R1 < len) 
  {
    kf = (R - R1) / len;
    dx = (INT)(kf * (Mx - X) + X);
    dy = (INT)(kf * (My - Y) + Y);
    Ellipse(hDC, dx - R1, dy - R1, dx + R1, dy + R1);
  }
  else
    Ellipse(hDC, Mx - R1, My - R1, Mx + R1, My + R1);

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  DeleteObject(hPen);
  DeleteObject(hBr);
}


/* END OF 't02eyes.c' FILE*/