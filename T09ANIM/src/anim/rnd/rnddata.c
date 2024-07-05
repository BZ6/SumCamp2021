/* FILE NAME: rnddata.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation rendering data module.
 */

#include "rnd.h"

/* Global rendering data */
HWND BZ6_hRndWnd;                 /* Work window handle */
HGLRC BZ6_hRndGLRC;               /* OpenGL render window device context  */
HDC BZ6_hRndDC;                   /* Render window device context */
INT BZ6_RndFrameW, BZ6_RndFrameH; /* Work window size */

FLT
  BZ6_RndProjSize = 0.1,     /* Project plane fit square */
  BZ6_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  BZ6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */

MATR
  BZ6_RndMatrView, /* View coordinate system matrix */
  BZ6_RndMatrProj, /* Projection coordinate system matrix */
  BZ6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* END OF 'rnddata.c' FILE */