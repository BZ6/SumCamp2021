/* FILE NAME: units.h
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D unit declaration module.
 */

#ifndef __units_h_
#define __units_h_

#include "../anim/anim.h"

extern VEC PosCowSecond;
extern VEC PosCowFirst;
extern INT Count;
extern INT RandomNumber;

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateBall( VOID );

/* Unit bounce ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateBounceBall( VOID );

/* Unit plane creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreatePlane( VOID );

/* Unit plane creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateCtrl( VOID );

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateCow( VOID );

/* Unit second cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateCowSecond( VOID );

/* Unit torus creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateTorus( VOID );

/* Unit fence creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateFence( VOID );

/* Unit plane creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateTex( VOID );

bz6UNIT * BZ6_UnitCreateCowRandom( VOID );

#endif /* __units_h_ */

/* END OF 'units.h' FILE */