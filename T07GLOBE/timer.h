/* FILE NAME: timer.h
 * PROGRAMMER: BZ6
 * DATE: 15.06.2021
 * PURPOSE: Timer declaration file.
 */

#ifndef __timer_h_
#define __timer_h_

#include <windows.h>

/* Global timer data */
extern DOUBLE 
  GLB_Time,              /* Time in sec */
  GLB_DeltaTime,         /* Interframe time in sec */ 
  GLB_FPS;               /* FPS */

extern BOOL GLB_IsPause; /* Flag pause */

/* Forward declaration */
VOID GLB_TimerInit( VOID );
VOID GLB_TimerResponse( VOID );

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE*/