/* FILE NAME: rnddata.c
 * PROGRAMMER: BZ6
 * DATE: 17.06.2021
 * PURPOSE: 3D animation rendering data module.
 */

#include "rnd.h"

/* Global rendering data */
HWND BZ6_hRndWnd;                 /* Work window handle */
HDC BZ6_hRndDCFrame;              /* Work window memory device context  */
HBITMAP BZ6_hRndBmFrame;          /* Work window background bitmap handle */
INT BZ6_RndFrameW, BZ6_RndFrameH; /* Work window size */

DBL
  BZ6_RndProjSize = 0.1,     /* Project plane fit square */
  BZ6_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  BZ6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */

MATR
  BZ6_RndMatrView, /* View coordinate system matrix */
  BZ6_RndMatrProj, /* Projection coordinate system matrix */
  BZ6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* END OF 'rnddata.c' FILE */