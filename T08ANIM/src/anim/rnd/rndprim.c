/* FILE NAME: rndprim.c
 * PROGRAMMER: BZ6
 * DATE: 17.06.2021
 * PURPOSE: 3D animation rendering primitive module.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "rnd.h"

/* Rendering create primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - numbers of vertexes and indexes:
 *       INT NoofV, NoofI
 * RETURNS:
 *   (BOOL) TRUE if complete, FALSE if not complete.
 */
BOOL BZ6_RndPrimCreate( bz6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(bz6PRIM));
  size = sizeof(bz6VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
} /* End of 'BZ6_RndPrimCreate' function */

/* Rendering free primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimFree( bz6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(bz6PRIM));
} /* End of 'BZ6_RndPrimFree' function */

/* Rendering draw primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - matrix of transformation primitive:
 *       MATR World;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimDraw( bz6PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, BZ6_RndMatrVP);
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* Build projection */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (INT)((p.X + 1) * BZ6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * BZ6_RndFrameH / 2);
  }

  /* Draw triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(BZ6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(BZ6_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(BZ6_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(BZ6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
} /* End of 'BZ6_RndPrimDraw' function */

/* Rendering draw sphere function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - location:
 *       VEC C;
 *   - radius:
 *       DBL R;
 *   - size:
 *       INT SplitW, SplitH;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimCreateSphere( bz6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j; 
  DBL tetha, phi;

  BZ6_RndPrimCreate(Pr, SplitW * SplitH, (SplitH - 1) * (SplitW - 1) * 6);

  for (i = 0, tetha = 0; i < SplitH; i++, tetha += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      Pr->V[j + SplitW * i].P = VecSet(C.X + R * sin(tetha) * sin(phi), C.Y + R * cos(tetha), C.Z + R * sin(tetha) * cos(phi));

  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH);
} /* End of 'BZ6_RndPrimCreateSphere' function */

/* Rendering draw base function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - location:
 *       VEC Position;
 *   - plane direction:
 *       VEC DirectionWeird, DirectionHeight;
 *   - size of base:
 *       DBL BaseWeird, BaseHeight;
 *   - size:
 *       INT SplitW, SplitH;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimCreateBase( bz6PRIM *Pr, VEC Position, VEC DirectionWeird, VEC DirectionHeight, DBL BaseWeird, DBL BaseHeight, INT SplitW, INT SplitH )
{
  INT i, j;
  VEC DirectionW = VecMulNum(VecNormalize(DirectionWeird), BaseWeird), DirectionH = VecMulNum(VecNormalize(DirectionHeight), BaseHeight);

  BZ6_RndPrimCreate(Pr, SplitW * SplitH, (SplitH - 1) * (SplitW - 1) * 6);

  for (i = 0; i < SplitH; i++)
    for (j = 0; j < SplitW; j++)
      Pr->V[j + SplitW * i].P = VecAddVec(Position, VecAddVec(VecMulNum(DirectionW, j),
                                          VecMulNum(DirectionH, i)));
  Pr->Trans = MatrTranslate(VecAddVec(VecMulNum(DirectionW, (DBL)(-SplitW) / 2), VecMulNum(DirectionH, (DBL)(-SplitH) / 2)));

  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH);
} /* End of 'BZ6_RndPrimCreatebase' function */

/* Create grid function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - size:
 *       INT GridW, GridH;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimCreateGrid( bz6PRIM *Pr, INT GridW, INT GridH )
{
  INT i, j, k;

  for (i = 0, k = 0; i < GridH - 1; i++)
    for (j = 0; j < GridW - 1; j++) 
    {
      Pr->I[k++] = j + GridW * i;
      Pr->I[k++] = j + GridW * i + 1;
      Pr->I[k++] = j + GridW * (i + 1);
      Pr->I[k++] = j + GridW * i + 1;
      Pr->I[k++] = j + GridW * (i + 1) + 1;
      Pr->I[k++] = j + GridW * (i + 1);
    }
} /* End of 'BZ6_RndPrimCreateGrid' function */

/* Rendering draw torus function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - location:
 *       VEC C;
 *   - radiuses small and big:
 *       DBL r, R;
 *   - size:
 *       INT SplitW, SplitH;
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimCreateTorus( bz6PRIM *Pr, VEC C, DBL r, DBL R, INT SplitW, INT SplitH )
{
  INT i, j; 
  DBL tetha, phi;

  BZ6_RndPrimCreate(Pr, SplitW * SplitH, (SplitH - 1) * (SplitW - 1) * 6);

  for (i = 0, tetha = 0; i < SplitH; i++, tetha += 2 * PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      Pr->V[j + SplitW * i].P = VecSet(C.X + (r * sin(phi) + R + r) * sin(tetha),
                                       C.Y + r * cos(phi),
                                       C.Z + (r * sin(phi) + R + r) * cos(tetha));

  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH);
} /* End of 'BZ6_RndPrimCreateTorus' function */

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       bz6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BZ6_RndPrimLoad( bz6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nind = 0;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(bz6PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertexes and indexes */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          n++;
      nind += (n - 2) * 3;
    }
  }

  if (!BZ6_RndPrimCreate(Pr, nv, nind))
  {
    fclose(F);
    return FALSE;
  }

  /* Load primitive */
  rewind(F);
  nv = 0;
  nind = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, n0, n1, nc;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
        {
          sscanf(Buf + i, "%i", &nc);
          if (nc < 0)
            nc = nv + nc;
          else
            nc--;

          if (n == 0)
            n0 = nc;
          else if (n == 1)
            n1 = nc;
          else
          {
            Pr->I[nind++] = n0;
            Pr->I[nind++] = n1;
            Pr->I[nind++] = nc;
            n1 = nc;
          }
          n++;
        }
    }
  }

  fclose(F);
  return TRUE;
} /* End of 'BZ6_RndPrimLoad' function */

/* END OF 'rndprim.c' FILE */