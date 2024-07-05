/* FILE NAME: timer.h
 * PROGRAMMER: BZ6
 * DATE: 15.06.2021
 * PURPOSE: Timer declaration file.
 */

#include <time.h>
#include "timer.h"

/* Global timer data */
DOUBLE 
  GLB_Time,         /* Time in sec */
  GLB_DeltaTime,    /* Interframe time in sec */ 
  GLB_FPS;          /* FPS */

BOOL GLB_IsPause;   /* Flag pause */

static LONG 
  StartTime,        /* Time start in sec */
  OldTime,          /* Time previous frame in sec */
  PauseTime,        /* Time in pause in sec */
  OldFPSTime,       /* Time previous FPS in sec */
  FrameCount;       /* Count of frame */

/* The timer initialization function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID GLB_TimerInit( VOID )
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  GLB_IsPause = FALSE;
} /* End of 'GLB_TimerInit' function */

/* The timer response function.
 * ARGUMENTS: NONE;
 * RETURNS: NONE.
 */
VOID GLB_TimerResponse( VOID )
{
  LONG t = clock();

  if (!GLB_IsPause)
  {
    GLB_Time = (DOUBLE)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DOUBLE)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }
  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount / ((DOUBLE)(t - OldFPSTime) / CLOCKS_PER_SEC);
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;    
} /* End of 'GLB_TimerResponse' function */


/* END OF 'timer.h' FILE*/