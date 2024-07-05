/* FILE NAME: game.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: game function module.
 */

#include "game.h"

/* Game initialization function.
 * ARGUMENTS:
 *   - handle window:
 *       HWND hWnd;
 * RETURNS:
 *   NONE.
 */
VOID GameInit( HWND hWnd )
{
  RandomNumber = 0;
  Count = 0;
  BZ6_AnimInit(hWnd);
} /* End of 'GameInit' function */


/* Game resize function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, H;
 * RETURNS:
 *   NONE.
 */
VOID GameResize( INT W, INT H )
{
  BZ6_AnimResize(W, H);
} /* End of 'GameResize' function */

/* Game render function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID GameRender( VOID )
{
  BZ6_AnimRender();
} /* End of 'GameRender' function */


/* Game copy frame function.
 * ARGUMENTS: NONE.
 * RETURNS:
 *   NONE.
 */
VOID GameCopyFrame( VOID )
{
  BZ6_AnimCopyFrame();
} /* End of 'GameCopyFrame' function */

/* Game close function.
 * ARGUMENTS:
 *   NONE;
 * RETURNS:
 *   NONE.
 */
VOID GameClose( VOID )
{
  BZ6_AnimClose();
} /* End of 'GameClose' function */

/* END OF 'game.c' FILE */