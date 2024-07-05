/* FILE NAME: u_cow.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation cow unit module.
 */

#include <stdio.h>
#include "units.h"

typedef struct
{
  BZ6_UNIT_BASE_FIELDS;
  VEC Pos;
  bz6PRIM Pr;
  VEC Dir;
  MATR Cow;
  BOOL IsDvig;
} bz6UNIT_COW;

/* Unit cow initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitInit( bz6UNIT_COW *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimLoad(&Uni->Pr, "BIN/MODELS/cow.obj");
  Uni->IsDvig = FALSE;
  Uni->Pos = VecSet(1, 0, 5);
  Uni->Dir = VecSet(0, 0 ,-1);
  Uni->Cow = MatrMulMatr(MatrScale(VecSet1(0.1)), MatrRotateY(90));
  BZ6_RndShadersClose();
  BZ6_RndShaderAdd("COW");
} /* End of 'BZ6_UnitInit' function */

/* Unit cow inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitResponse( bz6UNIT_COW *Uni, bz6ANIM *Ani )
{
  VEC ve;

  Uni->IsDvig = FALSE;

  Uni->Cow = MatrMulMatr(Uni->Cow, MatrRotate(Ani->DeltaTime * 400 * (Ani->Keys['A']- Ani->Keys['D']), VecSet(0, 1, 0)));
  Uni->Dir = VectorTransform(Uni->Dir, MatrRotate(Ani->DeltaTime * 400 * (Ani->Keys['A']- Ani->Keys['D']), VecSet(0, 1, 0)));
  ve = VecAddVec(Uni->Pos, VecMulNum(Uni->Dir, Ani->DeltaTime * 50 * (Ani->Keys['W']- Ani->Keys['S'])));

  if (ve.X < 47.5 && ve.X > -48.5 && ve.Z < 47.5 && ve.Z > -48.5)
    if ((ve.X < 1.5 + RandomNumber || ve.X > 6 + RandomNumber || ve.Z < -2.5 + RandomNumber || ve.Z > 2.5 + RandomNumber) && (ve.X < -6 + RandomNumber || ve.X > -1.5 + RandomNumber || ve.Z < -2.5 + RandomNumber || ve.Z > 2.5 + RandomNumber))
      Uni->IsDvig = TRUE;

  if (ve.X < 1.5 + RandomNumber && ve.Z > -2.5 + RandomNumber && ve.Z < 2.5 + RandomNumber && ve.X > -1.5 + RandomNumber && Uni->IsDvig)
      RandomNumber = rand() % 80 - 40;

  if (Uni->IsDvig)
    Uni->Pos = ve;
  else
    Uni->Pos = Uni->Pos;

  /*
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
    Uni->Pos = Uni->Pos;*/
} /* End of 'BZ6_UnitResponse' function */

/* Unit cow render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitRender( bz6UNIT_COW *Uni, bz6ANIM *Ani )
{
  static CHAR Buf[10];

  sprintf(Buf, "%i", RandomNumber);
  SetWindowText(Ani->hWnd, Buf);
  BZ6_RndPrimDraw(&Uni->Pr, MatrMulMatr(Uni->Cow, MatrTranslate(Uni->Pos)));
} /* End of 'BZ6_UnitRender' function */

/* Unit cow deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       bz6UNIT_COW *Uni;
 *   - animation context:
 *       bz6ANIM *Ani;
 * RETURNS: None.
 */
static VOID BZ6_UnitClose( bz6UNIT_COW *Uni, bz6ANIM *Ani )
{
  BZ6_RndPrimFree(&Uni->Pr);
} /* End of 'BZ6_UnitClose' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bz6UNIT *) pointer to created unit.
 */
bz6UNIT * BZ6_UnitCreateCow( VOID )
{
  bz6UNIT *Uni;

  if ((Uni = (bz6UNIT *)BZ6_AnimUnitCreate(sizeof(bz6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)BZ6_UnitInit;
  Uni->Response = (VOID *)BZ6_UnitResponse;
  Uni->Render = (VOID *)BZ6_UnitRender;
  Uni->Close = (VOID *)BZ6_UnitClose;

  return Uni;
} /* End of 'BZ6_UnitCreateCow' function */

/* END OF 'u_cow.c' FILE */