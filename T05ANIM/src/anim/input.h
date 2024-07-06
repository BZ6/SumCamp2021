/* FILE NAME  : input.h
 * PROGRAMMER : BZ66
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Input class.
 */


#include <cstring>

#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Get joystock axes macro function */
#define GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

#include "../zagl.h"

/* namespace zagl */
namespace zagl
{
  /* Mouse handle class */
  class mouse
  {
  private:
    HWND &hWnd;
    INT &MouseWheel;
  public:
    /* mouse responsing */
    INT MouseX, MouseY, MouseZ,    /* mouse coordinates */
        MouseDX, MouseDY, MouseDZ;

    /* Mouse comstructor */
    mouse( HWND &hWnd, INT &MouseWheel  ) : hWnd(hWnd), MouseWheel(MouseWheel)
    {
      MouseX = 0;
      MouseY = 0;
      MouseZ = 0;
      MouseDX = 0;
      MouseDY = 0;
      MouseDZ = 0;
    } /* End of 'mouse' function */

    /* Response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);

      /* transform coordinates on the screen */
      MouseDX = pt.x - MouseX;
      MouseDY = pt.y - MouseY;
      MouseDZ = MouseWheel;

      /* absolute val */
      MouseX = pt.x;
      MouseY = pt.y;
      MouseZ += MouseWheel;

      MouseWheel = 0;
    } /* End of 'Response' function */
  }; /* End of 'mouse' class */

  /* Keyboard handle class */
  class keyboard
  {
  private:
    BYTE KeysOld[256];   /* state of keys on the current frame */
  public:
    BYTE Keys[256];      /* state of keys on the current frame */
    BYTE KeysClick[256]; /* signs of a single click of the keyboard */

    /* Base keyboard constructor */
    keyboard( VOID )
    {
    }

    /* Response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      GetKeyboardState(Keys);
      for (INT i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];
      }
      for (INT i = 0; i < 256; i++)
        KeysOld[i] = Keys[i];
    } /* End of 'Response' function */
  };

  /* Joystick handle class */
  class joystick
  {
  public:
    /* joystick responsing */
    BYTE
      JBut[32], JButOld[32], JButClick[32]; /* joystick button states */
    INT JPov;                               /* joystick point-of-view control [-1, 0..7] */
    DBL JoyX;
    DBL JoyY;
    DBL JoyZ;
    DBL JoyR;               /* joystick axes */

    /* joystick constructor */
    joystick( VOID )
    {
      JoyX = 0;
      JoyY = 0;
      JoyZ = 0;
      JoyR = 0;
      JPov = 0;
    } /* End of 'joystick' function */

    /* Response function.
     * ARGUEMNTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      if (joyGetNumDevs() > 0)
      {
        JOYCAPS jc;

        /* get joysick info */
        if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
        {
          JOYINFOEX ji;

          ji.dwSize = sizeof(JOYINFOEX);
          ji.dwFlags = JOY_RETURNALL;
          if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
          {
            /* buttons */
            for (INT i = 0; i < 32; i++)
            {
              JBut[i] = (ji.dwButtons >> i) & 1;
              JButClick[i] = JBut[i] && !JButOld[i];
              JButOld[i] = JBut[i];
            }
            /* axes */
            JoyX = GET_JOYSTIC_AXIS(X);
            JoyY = GET_JOYSTIC_AXIS(Y);
            JoyZ = GET_JOYSTIC_AXIS(Z);
            JoyR = GET_JOYSTIC_AXIS(R);
          }
        }
      }
    } /* End of 'Respose' function */
  }; /* End of 'joystick' class */

  /* input class */
  class input : public mouse, public keyboard, public joystick
  {
  public:
    /* input constructor */
    input( HWND &hWnd, INT &MouseWheel ) : mouse(hWnd, MouseWheel), keyboard(), joystick()
    {
    } /* End of 'input' function */

    /* Response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      mouse::Response();
      keyboard::Response();
      joystick::Response();
    } /* End of 'Response' function */
  }; /* End of 'input' class */
} /* end of 'zagl' namespace */

/* END OF 'input.h' FILE */
