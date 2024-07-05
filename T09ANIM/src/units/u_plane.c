/* FILE NAME: u_plane.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation plane unit module.
 */

#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
  INT TexId;
} bz6UNIT_TEX;

/* Unit plane initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_TEX *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimCreateBase(&Uni->Pr, Uni->Pos, VecSet(1, 0, 0), VecSet(0, 0, 1), 1, 1, 100, 100);
} /* End of 'BZ6_UnitInit' function */

/* Unit plane render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_TEX *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimDraw(&Uni->Pr, MatrTranslate(VecSet(0, 0, 0)));
} /* End of 'BZ6_UnitRender' function */

/* Unit plane deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_TEX *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit plane creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreatePlane( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_TEX))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreatePlane' function */

/* END OF 'u_plane.c' FILE */