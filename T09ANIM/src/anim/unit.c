/* FILE NAME: unit.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D unit function module.
 */

#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitResponse( bz6UNIT *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_AnimUnitCreate( INT Size )
{
  bz6UNIT *Uni;


  /* Memory allocation */
  if (Size < sizeof(bz6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = BZ6_UnitInit;
  Uni->Close = BZ6_UnitClose;
  Uni->Response = BZ6_UnitResponse;
  Uni->Render = BZ6_UnitRender;

  return Uni;
} /* End of 'BZ6_AnimUnitCreate' function */

/* END OF 'unit.c' FILE */