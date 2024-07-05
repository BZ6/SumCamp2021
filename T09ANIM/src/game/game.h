/* FILE NAME: game.h
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D unit declaration module.
 */

#ifndef __game_h_ 
#define __game_h_ 

#include "../units/units.h"

/* Game initialization function.
 * ARGUMENTS:
 *   - handle window:
 *       HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID GameInit( HWND hWnd );

/* Game resize function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, H;
 * RETURNS:
 *   NONE.
 */
VOID GameResize( INT W, INT H );

/* Game render function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID GameRender( VOID );

/* Game copy frame function.
 * ARGUMENTS: NONE.
 * RETURNS:
 *   NONE.
 */
VOID GameCopyFrame( VOID );

/* Game close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID GameClose( VOID );

#endif /* __game_h_ */

/* END OF 'game.h' FILE */