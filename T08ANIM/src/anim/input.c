/* FILE NAME: input.c
 * PROGRAMMER: BZ6
 * DATE: 19.06.2021
 * PURPOSE: animation handle input function module.
 */

#pragma comment(lib, "winmm")

#include "anim.h"

#include <mmsystem.h>
#include <string.h>

#define BZ6_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

/* Global mouse wheel data */
INT BZ6_MouseWheel;

/* Keyboard initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimKeyboardInit( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
  {
    BZ6_Anim.KeysOld[i] = 0;
    BZ6_Anim.KeysClick[i] = 0;
    BZ6_Anim.Keys[i] = 0;
  }
} /* End of 'BZ6_AnimKeyboardInit' function */

/* Keyboard response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(BZ6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    BZ6_Anim.Keys[i] >>= 7;
    BZ6_Anim.KeysClick[i] = BZ6_Anim.Keys[i] && !BZ6_Anim.KeysOld[i];
  }
  memcpy(BZ6_Anim.KeysOld, BZ6_Anim.Keys, 256);
} /* End of 'BZ6_AnimKeyboardResponse' function */

/* Mouse initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(BZ6_Anim.hWnd, &pt);

  /* Absolute coordinates in first frame */
  BZ6_Anim.Mx = pt.x;
  BZ6_Anim.My = pt.y;
  BZ6_Anim.Mz = 0;
} /* End of 'BZ6_AnimMouseInit' function */

/* Mouse response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(BZ6_Anim.hWnd, &pt);

  /* Delta coordinates in frame */
  BZ6_Anim.Mdx = pt.x - BZ6_Anim.Mx;
  BZ6_Anim.Mdy = pt.y - BZ6_Anim.My;
  BZ6_Anim.Mdz = BZ6_MouseWheel;

  /* Absolute coordinates in frame */
  BZ6_Anim.Mx = pt.x;
  BZ6_Anim.My = pt.y;
  BZ6_Anim.Mz += BZ6_MouseWheel;

  BZ6_MouseWheel = 0;
} /* End of 'BZ6_AnimMouseResponse' function */

/* Joystick initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimJoystickInit( VOID )
{
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;


    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;


      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        INT i;

        /* Buttons */
        for (i = 0; i < 32; i++)
          BZ6_Anim.JButOld[i] = (ji.dwButtons >> i) & 1;
        /* Axes */
        BZ6_Anim.JX = BZ6_GET_JOYSTIC_AXIS(X);
        BZ6_Anim.JY = BZ6_GET_JOYSTIC_AXIS(Y);
        BZ6_Anim.JZ = BZ6_GET_JOYSTIC_AXIS(Z);
        BZ6_Anim.JR = BZ6_GET_JOYSTIC_AXIS(R);
        /* Point of view */
        BZ6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
} /* End of 'BZ6_AnimJoystickInit' function */

/* Joystick response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
static VOID BZ6_AnimJoystickResponse( VOID )
{
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;


    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;


      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        INT i;

        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          BZ6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          BZ6_Anim.JButClick[i] = BZ6_Anim.JBut[i] && !BZ6_Anim.JButOld[i];
          BZ6_Anim.JButOld[i] = BZ6_Anim.JBut[i];
        }
        /* Axes */
        BZ6_Anim.JX = BZ6_GET_JOYSTIC_AXIS(X);
        BZ6_Anim.JY = BZ6_GET_JOYSTIC_AXIS(Y);
        BZ6_Anim.JZ = BZ6_GET_JOYSTIC_AXIS(Z);
        BZ6_Anim.JR = BZ6_GET_JOYSTIC_AXIS(R);
        /* Point of view */
        BZ6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
} /* End of 'BZ6_AnimJoystickResponse' function */

/* Animation input initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_AnimInputInit( VOID )
{
  BZ6_AnimKeyboardInit();
  BZ6_AnimMouseInit();
  BZ6_AnimJoystickInit();
} /* End of 'BZ6_AnimInputInit' function */

/* Animation input response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_AnimInputResponse( VOID )
{
  BZ6_AnimKeyboardResponse();
  BZ6_AnimMouseResponse();
  BZ6_AnimJoystickResponse();
} /* End of 'BZ6_AnimInputResponse' function */

/* END OF 'timer.c' FILE */
