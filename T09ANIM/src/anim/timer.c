/* FILE NAME: timer.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: timer function module.
 */

#include "anim.h"

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

/* Timer initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  BZ6_Anim.IsPause = FALSE;
  BZ6_Anim.FPS = 30.0;
  PauseTime = 0;
} /* End of 'BZ6_TimerInit' function */

/* Timer interframe response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID BZ6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  BZ6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  BZ6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (BZ6_Anim.IsPause)
  {
    BZ6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    BZ6_Anim.DeltaTime = BZ6_Anim.GlobalDeltaTime;
    BZ6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    BZ6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'BZ6_TimerResponse' function */

/* END OF 'timer.c' FILE */