/* FILE NAME: u_fence.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation fence unit module.
 */

#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
} bz6UNIT_FENCE;

/* Unit fence initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_FENCE *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_FENCE *Uni, bz6ANIM *Ani )
{
  Uni->Pos = VecSet1(0);
  BZ6_RndPrimCreateBase(&Uni->Pr, Uni->Pos, VecSet(0, 1, 0), VecSet(0, 0, 1), 1, 1, 5, 100);
} /* End of 'BZ6_UnitInit' function */

/* Unit fence render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_FENCE *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_FENCE *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimDraw(&Uni->Pr, MatrTranslate(VecSet(49, 2.5, 0)));
  BZ6_RndPrimDraw(&Uni->Pr, MatrTranslate(VecSet(-50, 2.5, 0)));
  BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr(MatrRotateY(90), MatrTranslate(VecSet(0, 2.5, 49))));
  BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr(MatrRotateY(90), MatrTranslate(VecSet(0, 2.5, -50))));
} /* End of 'BZ6_UnitRender' function */

/* Unit fence deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_FENCE *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_FENCE *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit fence creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateFence( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_FENCE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreateFence' function */

/* END OF 'u_fence.c' FILE */