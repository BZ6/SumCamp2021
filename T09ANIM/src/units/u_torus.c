/* FILE NAME: u_torus.c
 * PROGRAMMER: BZ6
 * DATE: 23.06.2021
 * PURPOSE: 3D animation torus unit module.
 */

#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
} bz6UNIT_TORUS;


INT Count;
INT RandomNumber;

/* Unit torus initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_TORUS *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_TORUS *Uni, bz6ANIM *Ani )
{ 

  Uni->Pos = VecSet(0, 0, 0);
  BZ6_RndPrimCreateTorus(&Uni->Pr, Uni->Pos, 1, 3, 30, 20);
} /* End of 'BZ6_UnitInit' function */

/* Unit torus inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_TORUS *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitResponse( bz6UNIT_TORUS *Uni, bz6ANIM *Ani )
{
} /* End of 'BZ6_UnitResponse' function */

/* Unit torus render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_TORUS *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_TORUS *Uni, bz6ANIM *Ani )
{
  if (RandomNumber == 0)
    RandomNumber = rand() % 80 - 40;
  BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrRotateX(-90), MatrRotateZ(-90), MatrTranslate(VecSet(RandomNumber, -0.5, RandomNumber))));
  /*
  if (Count == 0)
    BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrRotateX(-90), MatrRotateZ(-90), MatrTranslate(VecSet(0, -0.5, 0))));

  if (Count == 1)
    BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrRotateX(-90), MatrRotateZ(-90), MatrTranslate(VecSet(20, -0.5, 10))));

  if (Count == 2)
    BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrRotateX(-90), MatrRotateZ(-90), MatrTranslate(VecSet(-20, -0.5, -10))));  */
} /* End of 'BZ6_UnitRender' function */

/* Unit torus deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_TORUS *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_TORUS *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit torus creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateTorus( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_TORUS))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Response = (VOID *)BZ6_UnitResponse;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreateTorus' function */

/* END OF 'u_torus.c' FILE */