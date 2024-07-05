/* t01fwin.c
 */
#include <windows.h>

#define WND_CLASS_NAME "my window"


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam);

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

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 10, NULL);
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);  
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    SelectObject(hDC, hPen);
    Rectangle(hDC, 0, 0, w, h);
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hDC, hPen);
    MoveToEx(hDC, w / 2, h / 2, NULL);
    LineTo(hDC, pt.x, pt.y);
    SelectObject(hDC, GetStockObject(DC_PEN));
    DeleteObject(hPen);

    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;
  case WM_PAINT:
    BeginPaint(hWnd, &ps);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* END OF 't01fwin.c' FILE*/