/* FILE NAME: rndbase.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation rendering base module.
 */

#include "../anim.h"

/* Rendering initialization function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndInit( HWND hWnd )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  BZ6_hRndWnd = hWnd;
  BZ6_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(BZ6_hRndDC, &pfd);
  DescribePixelFormat(BZ6_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(BZ6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  BZ6_hRndGLRC = wglCreateContext(BZ6_hRndDC);
  wglMakeCurrent(BZ6_hRndDC, BZ6_hRndGLRC);

  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(BZ6_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(BZ6_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(BZ6_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(BZ6_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(BZ6_hRndGLRC);

  BZ6_hRndGLRC = hRC;
  wglMakeCurrent(BZ6_hRndDC, BZ6_hRndGLRC);
  /* Set default OpenGL parameters */
  glEnable(GL_DEPTH_TEST);
  wglSwapIntervalEXT(0);
  glClearColor(0.30, 0.47, 0.8, 1);

  BZ6_RndShadersInit();
  BZ6_RndTexInit();

  BZ6_hRndWnd = hWnd;

  BZ6_RndFrameW = BZ6_RndFrameH = 100;
  BZ6_RndMatrView = MatrIdentity();
  BZ6_RndMatrProj = MatrIdentity();
  BZ6_RndMatrVP = MatrIdentity();
  BZ6_RndCamSet(VecSet(2, 3, 5), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'BZ6_RndInit' function */

/* Renderin   g close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndClose( VOID )
{
  BZ6_RndTexClose();
  BZ6_RndShadersClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(BZ6_hRndGLRC);
  ReleaseDC(BZ6_hRndWnd, BZ6_hRndDC);
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
  glViewport(0, 0, W, H);

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
VOID BZ6_RndCopyFrame( VOID )
{
  SwapBuffers(BZ6_hRndDC);
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
  static DBL dt;

  dt += BZ6_Anim.GlobalDeltaTime;
  if (dt > 2)
  {
    dt = 0;
    BZ6_RndShadersUpdate();
  }

  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} /* End of 'BZ6_RndStart' function */

/* Rendering end function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndEnd( VOID )
{
  glFinish();
} /* End of 'BZ6_RndEnd' function */

/* END OF 'rndbase.c' FILE */