/* FILE NAME  : win.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Win class declaration module.
 */

#ifndef __win_h_
#define __win_h_

#include <windowsx.h>

#include "../def.h"

/* Namespace zart */
namespace zart
{
  class win
  {
  protected:
    HINSTANCE hInstance; // application handle
    INT MouseWheel;      // Mouse wheel

  private:
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

    BOOL IsFullScreen;       // Full screen flag
    RECT FullScreenSaveRect; // Screen rectangle
    
    BOOL IsInit;

    const INT InitTimer = 30;
    const INT RefreshTimer = 0;
  public:
    INT W, H;            // window size
    BOOL IsActive; // Is window active flag
    HWND hWnd;           // window handle

    win( HINSTANCE hInst = GetModuleHandle(NULL) );
    ~win( VOID );

    VOID Run( VOID );
    VOID FlipFullScreen( VOID );

  private:
    // callback functions
    virtual VOID Init( VOID )
    {
    }

    virtual VOID Close( VOID )
    {
    }

    virtual VOID Resize( INT W, INT H )
    {
    }

    virtual VOID Timer( VOID )
    {
    }

    virtual VOID Activate( VOID )
    {
    }

    virtual VOID Idle( VOID )
    {
    }

    virtual VOID Erase( HDC hDC )
    {
    }

    virtual VOID Paint( HDC hDC )
    {
    }

    // message cracks
    VOID OnGetMinMaxInfo( MINMAXINFO *MinMax );
    BOOL OnCreate( CREATESTRUCT *CS );
    VOID OnSize( UINT State, INT W, INT H );
    VOID OnTimer( INT Id );
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );
    VOID OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys );
    VOID OnButtonUp( INT X, INT Y, UINT Keys );
    BOOL OnEraseBkgnd( HDC hDC );
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS );
    VOID OnClose( VOID );
    VOID OnDestroy( VOID );
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );
  };
}

#endif /* __win_h_ */

/* END OF 'win.h' FILE */
