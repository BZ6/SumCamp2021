/* FILE NAME  : main.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 04.08.2021
 * PURPOSE    : Entry point.
 */

#include "rt/raytracer.h"

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
  zart::raytracer win;
  
  win.LoadData(std::string("in"));

  win.Run();
  return 30;
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */