/* FILE NAME  : timer.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Timer class.
 */

#include "../zagl.h"

/* zagl namespace */
namespace zagl
{
  /* Timer class */
  class timer
  {
  private:
    UINT64
      StartTime,    // Start program time
      OldTime,      // Previous frame time
      OldTimeFPS,   // Old time FPS measurement
      PauseTime,    // Time during pause period
      TimePerSec,   // Timer resolution
      FrameCounter; // Frames counter
  public:
    DBL
      GlobalTime, GlobalDeltaTime, // Global time and interframe interval
      Time, DeltaTime,             // Time with pause and interframe interval
      FPS;                         // Frame per second
      BOOL IsPause;                // Pause flag


    /* Timer constructor function.
     * ARGUMETNS: None.
     * RETURNS: None.
     */
    timer( VOID ): DeltaTime(0), GlobalDeltaTime(0), GlobalTime(0), Time(0)
    {
      LARGE_INTEGER t;

      QueryPerformanceFrequency(&t);
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
      FrameCounter = 0;
      IsPause = FALSE;
      FPS = 30.0;
      PauseTime = 0;
    } /* End of 'timer' function */

    /* Timer interframe response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter(&t);
      /* Global time */
      GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
      /* Time with pause */
      if (IsPause)
      {
        DeltaTime = 0;
        PauseTime += t.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      /* FPS */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;
      }
      OldTime = t.QuadPart;
    } /* End of 'Response' function */
  }; /* End of 'timer' class */
} /* end of 'zagl' namespace */

/* END OF 'timer.h' FILE */
