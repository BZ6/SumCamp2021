/* FILE NAME: rndbase.c
 * PROGRAMMER: BZ6
 * DATE: 17.06.2021
 * PURPOSE: 3D animation rendering base module.
 */

#include "rnd.h"

/* Rendering initialization function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);

  BZ6_hRndWnd = hWnd;
  BZ6_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(BZ6_hRndWnd, hDC);
  BZ6_hRndBmFrame = NULL;
  BZ6_RndFrameW = BZ6_RndFrameH = 100;
  BZ6_RndMatrView = MatrIdentity();
  BZ6_RndMatrProj = MatrIdentity();
  BZ6_RndMatrVP = MatrIdentity();
  BZ6_RndCamSet(VecSet(2, 3, 5), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'BZ6_RndInit' function */

/* Rendering close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndClose( VOID )
{
  SelectObject(BZ6_hRndDCFrame, GetStockObject(DC_PEN));
  SelectObject(BZ6_hRndDCFrame, GetStockObject(DC_BRUSH));
  DeleteDC(BZ6_hRndDCFrame);
  DeleteObject(BZ6_hRndBmFrame);
} /* End of 'BZ6_RndClose' function */

/* Rendering projection set function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = BZ6_RndProjSize;

  /* Correct aspect ratio */
  if (BZ6_RndFrameW > BZ6_RndFrameH)
    rx *= (DBL)BZ6_RndFrameW / BZ6_RndFrameH;
  else
    ry *= (DBL)BZ6_RndFrameH / BZ6_RndFrameW;

  BZ6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      BZ6_RndProjDist, BZ6_RndProjFarClip);
  BZ6_RndMatrVP = MatrMulMatr(BZ6_RndMatrView, BZ6_RndMatrProj);
} /* End of 'BZ6_RndProjSet' function */

/* Rendering iresize function.
 * ARGUMENTS:
 *   - window size:
 *      INT W, H;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(BZ6_hRndWnd);

  if (BZ6_hRndBmFrame)
    DeleteObject(BZ6_hRndBmFrame);
  BZ6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(BZ6_hRndWnd, hDC);
  SelectObject(BZ6_hRndDCFrame, BZ6_hRndBmFrame);

  /* Save size */
  BZ6_RndFrameW = W;
  BZ6_RndFrameH = H;

  /* Reset projection */
  BZ6_RndProjSet();
} /* End of 'BZ6_RndResize' function */

/* Rendering copy frame function.
 * ARGUMENTS:
 *   - window handle discreaptor:
 *      HDC hDC;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, BZ6_RndFrameW, BZ6_RndFrameH,
    BZ6_hRndDCFrame, 0, 0, SRCCOPY);
} /* End of 'BZ6_RndCopyFrame' function */

/* Rendering look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  BZ6_RndMatrView = MatrView(Loc, At, Up);
  BZ6_RndMatrVP = MatrMulMatr(BZ6_RndMatrView, BZ6_RndMatrProj);
} /* End of 'BZ6_RndCamSet' function */

/* Rendering start function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndStart( VOID )
{ 
  static CHAR Buf[100];

  SelectObject(BZ6_hRndDCFrame, GetStockObject(WHITE_PEN));
  SelectObject(BZ6_hRndDCFrame, GetStockObject(WHITE_BRUSH));
  Rectangle(BZ6_hRndDCFrame, 0, 0, BZ6_RndFrameW, BZ6_RndFrameH);
  
  SelectObject(BZ6_hRndDCFrame, GetStockObject(BLACK_PEN));
  SelectObject(BZ6_hRndDCFrame, GetStockObject(BLACK_BRUSH));
} /* End of 'BZ6_RndStart' function */

/* Rendering end function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndEnd( VOID )
{
} /* End of 'BZ6_RndEnd' function */

/* END OF 'rndbase.c' FILE */