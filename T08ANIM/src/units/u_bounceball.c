/* FILE NAME: u_bounceball.c
 * PROGRAMMER: BZ6
 * DATE: 19.06.2021
 * PURPOSE: 3D animation bounce ball unit module.
 */

#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
} bz6UNIT_BOUNCE_BALL;

/* Unit bounce ball unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_BOUNCE_BALL *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimCreateSphere(&Uni->Pr, Uni->Pos, 0.5, 30, 20);
  Uni->Pos = VecSet1(0);
} /* End of 'BZ6_UnitInit' function */

/* Unit bounce ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitResponse( bz6UNIT_BOUNCE_BALL *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitResponse' function */

/* Unit bounce ball render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_BOUNCE_BALL *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimDraw(&Uni->Pr, MatrTranslate(VecSet(-2, 1.7 * fabs(sin(BZ6_Anim.Time * 5)), 0)));
} /* End of 'BZ6_UnitRender' function */

/* Unit bounce ball deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_BALL *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_BOUNCE_BALL *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit bounce ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateBounceBall( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_BOUNCE_BALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Response = (VOID *)BZ6_UnitResponse;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreateBounceBall' function */

/* END OF 'u_bounceball.c' FILE */