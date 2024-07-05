/* FILE NAME: u_cowsecond.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation second cow unit module.
 */

#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
  VEC Dir;
  MATR Cow;
  BOOL IsDvig;
} bz6UNIT_COW_RANDOM;

/* Unit cow initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW_RANDOM *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_COW_RANDOM *Uni, bz6ANIM *Ani )
{
  Uni->IsDvig = FALSE;
  BZ6_RndPrimLoad(&Uni->Pr, "BIN/MODELS/cow.obj");
  Uni->Pos = VecSet(-1, 0, 5);
  Uni->Dir = VecSet(0, 0, -1);
  Uni->Cow = MatrMulMatr(MatrScale(VecSet1(0.1)), MatrRotateY(90));
} /* End of 'BZ6_UnitInit' function */

/* Unit cow inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW_RANDOM *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitResponse( bz6UNIT_COW_RANDOM *Uni, bz6ANIM *Ani )
{
  VEC ve;

  Uni->IsDvig = FALSE;
  Uni->Cow = MatrMulMatr(Uni->Cow, MatrRotate(Ani->DeltaTime * 400 * (Ani->Keys[VK_LEFT]- Ani->Keys[VK_RIGHT]), VecSet(0, 1, 0)));
  Uni->Dir = VectorTransform(Uni->Dir, MatrRotate(Ani->DeltaTime * 400 * (Ani->Keys[VK_LEFT]- Ani->Keys[VK_RIGHT]), VecSet(0, 1, 0)));
  ve = VecAddVec(Uni->Pos, VecMulNum(Uni->Dir, Ani->DeltaTime * 50 * (Ani->Keys[VK_UP]- Ani->Keys[VK_DOWN])));
  
  if (ve.X < 47.5 && ve.X > -48.5 && ve.Z < 47.5 && ve.Z > -48.5)
    if ((ve.X < 1.5 || ve.X > 6 || ve.Z < -2.5 || ve.Z > 2.5) && (ve.X < -6 || ve.X > -1.5 || ve.Z < -2.5 || ve.Z > 2.5) && Count == 0)
      Uni->IsDvig = TRUE;
    else if ((ve.X < 21.5 || ve.X > 26 || ve.Z < 7.5 || ve.Z > 12.5) && (ve.X < 14 || ve.X > 19.5 || ve.Z < 7.5 || ve.Z > 12.5) && Count == 1)
      Uni->IsDvig = TRUE;
    else if ((ve.X < -19.5 || ve.X > -14 || ve.Z < -12.5 || ve.Z > -7.5) && (ve.X < -26 || ve.X > -21.5 || ve.Z < -12.5 || ve.Z > -7.5) && Count == 2)
      Uni->IsDvig = TRUE;

  if (Count == 0)
    if (ve.X < 1.5 && ve.Z > -2.5 && ve.Z < 2.5 && ve.X > -1.5 && Uni->IsDvig)
      Count = 1;

  if (Count == 1)
    if (ve.X < 21.5 && ve.Z > 7.5 && ve.Z < 12.5 && ve.X > 19.5 && Uni->IsDvig)
      Count = 2;

  if (Count == 2)
    if (ve.X < -19.5 && ve.Z > -12.5 && ve.Z < -7.5 && ve.X > -21.5 && Uni->IsDvig)
      Count = 0;


  if (Uni->IsDvig)
    Uni->Pos = ve;
  else
    Uni->Pos = Uni->Pos;

} /* End of 'BZ6_UnitResponse' function */

/* Unit cow render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW_RANDOM *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_COW_RANDOM *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr(Uni->Cow, MatrTranslate(Uni->Pos)));
} /* End of 'BZ6_UnitRender' function */

/* Unit cow deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW_RANDOM *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_COW_RANDOM *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit second cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateCowSecond( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_COW_RANDOM))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Response = (VOID *)BZ6_UnitResponse;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreateCowSecond' function */

/* END OF 'u_cowsecond.c' FILE */