/* FILE NAME: globe.h
 * PROGRAMMER: BZ6
 * DATE: 15.06.2021
 * PURPOSE: Declaration file.
 */

#ifndef __globe_h_
#define __globe_h_

#include <windows.h>
#include "timer.h"
#include "mth.h"

/* Global definitions */
#define GRID_H 30
#define GRID_W 50

/* Forward declaration */
VOID GlobeSet( INT w, INT h );
VOID GlobeDraw( HDC hDC, DBL Size );
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VEC RotateZ( DBL Angle, VEC V );
VEC RotateX( DBL Angle, VEC V );
VEC RotateY( DBL Angle, VEC V );

#endif /* __globe_h_ */

/* END OF 'globe.h' FILE*/