/* FILE NAME: units.h
 * PROGRAMMER: BZ6
 * DATE: 18.06.2021
 * PURPOSE: 3D unit declaration module.
 */

#ifndef __units_h_
#define __units_h_

#include "../anim/anim.h"

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

#endif /* __units_h_ */

/* END OF 'units.h' FILE */