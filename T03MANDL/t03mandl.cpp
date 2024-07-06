/* FILE: t03mandl.cpp
 * PROGRAMMER: BZ6
 * DATE: 19.07.2021
 * PURPOSE: Mandelbrot & Julia set drawing program.
 */
#include <ctime>
#include <math.h>

#include <glut.h>

#include <commondf.h>

#include <thread>

#define FRAME_W 512
#define FRAME_H 512

struct cmpl
{
  FLOAT a, b;

  cmpl( VOID ) : a(0), b(0)
  {
  }
  cmpl( FLOAT x, FLOAT y ): a(x), b(y)
  {
  }
  ~cmpl( VOID )
  {
  }
  cmpl operator+( const cmpl &c ) const
  {
    return cmpl(a + c.a, b + c.b);
  }
  cmpl operator+( FLOAT N )
  {
    return cmpl(a + N, b);
  }
  cmpl operator*( const cmpl &c ) const
  {
    return cmpl(a * c.a - b * c.b, a * c.b + b * c.a);
  }
  float operator!( VOID ) const
  {
    return a * a + b * b;
  }
  INT Julia( cmpl Z, cmpl C )
  {
    INT i = 0;
    cmpl tmp = Z;

    while (i++ <= 63 && !tmp < 4)
      tmp = tmp * tmp + C;
    return i;
  }
};

struct frame
{
  UCHAR Frame[FRAME_H][FRAME_W][4];
  DOUBLE Zoom;

  frame( VOID ) : Zoom(2)
  {
    memset(Frame, 0, FRAME_H * FRAME_W * 4);
  }
}Fr;
VOID DrawJulia( INT I )
  {
    DOUBLE t = (DOUBLE)clock() / CLOCKS_PER_SEC;
    cmpl sico( sin( t ), cos( t ) );
    INT i, j, H = FRAME_H, W = FRAME_W;

    if (I == 0)
      i = 0, j = 0, H /= 3, W /= 3;
    else if (I == 1)
      i = H / 3, j = 0, H = H / 3 * 2, W /= 3;
    else if (I == 2)
      i = H / 3 * 2, j = 0, W /= 3;
    else if (I == 3)
      i = 0, j = H / 3, H /= 3, W = H / 3 * 2;
    else if (I == 4)
      i = H / 3, j = H / 3, H = H / 3 * 2, W = H / 3 * 2;
    else if (I == 5)
      i = H / 3 * 2, j = H / 3, W = H / 3 * 2;
    else if (I == 6)
      i = 0, j = H / 3 * 2, H /= 3;
    else if (I == 7)
      i = H / 3, j = H / 3 * 2, H = H / 3 * 2;
    else if (I == 8)
      i = H / 3 * 2, j = H / 3 * 2;

    for (; i < H; i++)
      for (; j < W; j++)
      {
        INT Jul = sico.Julia(cmpl( (FLOAT)i / FRAME_H * 4 - 2, (FLOAT)j / FRAME_W * 4 - 2 ), sico );

        Fr.Frame[i][j][0] = Jul * 2;
        Fr.Frame[i][j][1] = Jul * 6;
        Fr.Frame[i][j][2] = Jul * 4;
      }
  }
VOID Display( VOID )
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  std::thread Th[9];

  for (INT i = 0; i < 9; i++)
    Th[i] = std::thread(DrawJulia, i);
  for (INT i = 0; i < 9; i++)
    Th[i].join();

  glRasterPos2d(-1, 1);
  glPixelZoom(Fr.Zoom, -Fr.Zoom);
  glDrawPixels(FRAME_W, FRAME_H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Fr.Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

/* The program entry point */
INT main( INT ArgC, CHAR *ArgV[] )
{
  glutInit(&ArgC, ArgV);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(720, 720);
  glutCreateWindow("Window!!!!");

  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}/* End of 'main' function */


/* END OF 't03mandl.cpp' FILE */