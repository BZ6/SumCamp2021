/* FILE NAME: anim.h
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation declaration module.
 */

#ifndef __anim_h_
#define __anim_h_

#include "rnd/res/rndres.h"

typedef struct tagbz6UNIT bz6UNIT;
typedef struct tagbz6ANIM bz6ANIM;

#define BZ6_MAX_UNITS 3000

#define BZ6_UNIT_BASE_FIELDS \
  VOID (*Init)( bz6UNIT *Uni, bz6ANIM *Ani );      \
  VOID (*Close)( bz6UNIT *Uni, bz6ANIM *Ani );     \
  VOID (*Response)( bz6UNIT *Uni, bz6ANIM *Ani );  \
  VOID (*Render)( bz6UNIT *Uni, bz6ANIM *Ani )

struct tagbz6UNIT
{
  BZ6_UNIT_BASE_FIELDS;
};

struct tagbz6ANIM
{
  HWND hWnd;                     /* Window */
  INT W, H;                      /* Window size */


  bz6UNIT *Units[BZ6_MAX_UNITS]; /* Array of units */
  INT NumOfUnits;                /* Count of units */

  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */

  /* Input data */
  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */
  BYTE 
    Keys[256],      /* Keyboard in frame */
    KeysClick[256], /* Keyboard 1 click in frame */
    KeysOld[256];   /* Keyboard in priveous frame */
  INT Mx, My, Mz, Mdx, Mdy, Mdz; /* Mouse data */
};

extern bz6ANIM BZ6_Anim;

extern INT BZ6_MouseWheel;

/* Animation initialization function.
 * ARGUMENTS:
 *   - handle window:
 *       HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimInit( HWND hWnd );
 
/* Animation close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimClose( VOID );

/* Animation resize function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, H;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimResize( INT W, INT H );

/* Animation copy frame function.
 * ARGUMENTS:
 *   - handle descriptor:
 *       HDC hDC;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimCopyFrame( VOID );

/* Animation render function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimRender( VOID );

/* Unit add in animation function.
 * ARGUMENTS:
 *   - pointer to created unit:
 *       bz6UNIT *Uni;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimAddUnit( bz6UNIT *Uni );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_AnimUnitCreate( INT Size );

/* Toggle window fullscreen mode function.
 * ARGUMENTS: NONE;
 * RETURNS: None.
 */
VOID BZ6_AnimFlipFullScreen( VOID );

/* Toggle window axit mode function.
 * ARGUMENTS: NONE;
 * RETURNS: None.
 */
VOID BZ6_AnimExit( VOID );

/***
 * Timer declaration
 ***/

/* Timer initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_TimerInit( VOID );

/* Timer interframe response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_TimerResponse( VOID );

/***
 * Input declaration
 ***/

/* Animation input initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_AnimInputInit( VOID );

/* Animation input response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_AnimInputResponse( VOID );

/* Animation unit delete function.
 * ARGUMENTS:
 *   NONE.
 * RETURNS:
 *   NONE.
 */
VOID BZ6_AnimDelUnit( VOID );

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */