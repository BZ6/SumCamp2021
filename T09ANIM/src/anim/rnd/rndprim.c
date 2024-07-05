/* FILE NAME: rndprim.c
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation rendering primitive module.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../anim.h"

/* Primitive creation function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       bz6PRIM *Pr;
 *   - vertex attributes array:
 *       bz6VERTEX *V;
 *   - number of vertices:
 *       INT NumOfV;
 *   - index array (for trimesh – by 3 ones, may be NULL)
 *       INT *I;
 *   - number of indices
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID BZ6_RndPrimCreate( bz6PRIM *Pr, bz6VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(bz6PRIM));   /* <-- <string.h> */

  if (V != NULL && NumOfV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bz6VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(bz6VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(bz6VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(bz6VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(bz6VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
  }

  if (I != NULL && NumOfI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NumOfI;
  }
  else
    Pr->NumOfElements = NumOfV;
  Pr->Trans = MatrIdentity();
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
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
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
  MATR wvp = MatrMulMatr3(Pr->Trans, World, BZ6_RndMatrVP);
  INT ProgId = BZ6_RndShaders[0].ProgId;
  INT loc;

  glUseProgram(ProgId);

  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, BZ6_Anim.Time);
  if ((loc = glGetUniformLocation(ProgId, "Tex")) != -1)
    glUniform1i(loc, 1);

  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);
  
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);

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
  bz6VERTEX *V;

  if ((V = malloc(sizeof(bz6VERTEX) * SplitW * SplitH)) == NULL)
    return;

  for (i = 0, tetha = 0; i < SplitH; i++, tetha += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {
      V[j + SplitW * i].P = VecSet(C.X + R * sin(tetha) * sin(phi), C.Y + R * cos(tetha), C.Z + R * sin(tetha) * cos(phi));
      V[j + SplitW * i].C = Vec4Set(0.3, 0.7, 0.5, 1);
    }

  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
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
  bz6VERTEX *V;

  if ((V = malloc(sizeof(bz6VERTEX) * SplitW * SplitH)) == NULL)
    return;

  for (i = 0; i < SplitH; i++)
    for (j = 0; j < SplitW; j++)
    {
      V[j + SplitW * i].P = VecAddVec(Position, VecAddVec(VecMulNum(DirectionW, j),
                                          VecMulNum(DirectionH, i)));
      V[j + SplitW * i].C = Vec4Set(0.5, 0.3, 0.7, 1);
    }
  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  Pr->Trans = MatrTranslate(VecAddVec(VecMulNum(DirectionW, (DBL)(-SplitW) / 2), VecMulNum(DirectionH, (DBL)(-SplitH) / 2)));
  free(V);
} /* End of 'BZ6_RndPrimCreatebase' function */

/* Create grid function.
 * ARGUMENTS:
 *   - primitive:
 *       bz6PRIM *Pr;
 *   - size:
 *       INT GridW, GridH;
 *   - vertexes:
 *       bz6VERTEX *V
 * RETURNS:
 *   NONE.
 */
VOID BZ6_RndPrimCreateGrid( bz6PRIM *Pr, INT GridW, INT GridH, bz6VERTEX *V )
{
  INT i, j, k;
  INT *Ind;
   
  if ((Ind = malloc(sizeof(INT) * (GridH - 1) * (GridW - 1) * 6)) == NULL)
    return;

  for (i = 0, k = 0; i < GridH - 1; i++)
    for (j = 0; j < GridW - 1; j++) 
    {
      Ind[k++] = j + GridW * i;
      Ind[k++] = j + GridW * i + 1;
      Ind[k++] = j + GridW * (i + 1);
      Ind[k++] = j + GridW * i + 1;
      Ind[k++] = j + GridW * (i + 1) + 1;
      Ind[k++] = j + GridW * (i + 1);
    }

  for (i = 0; i < GridH * GridW; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < (GridH - 1) * (GridW - 1) * 6; i+= 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p2 = V[Ind[i + 1]].P,
      p1 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

      V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
      V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
      V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }
  for (i = 0; i < GridH * GridW; i++)
  {
    VEC L = {1, 1, 1};
    FLT nl = VecDotVec(VecNormalize(V[i].N), L);
    
    if (nl < 0.1)
      nl = 0.1;
    V[i].C = Vec4Set(0.3 * nl, 0.995 * nl, 0.991 * nl, 1);
  }

  BZ6_RndPrimCreate(Pr, V, GridW * GridH, Ind, (GridH - 1) * (GridW - 1) * 6);
  free(Ind);
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
  bz6VERTEX *V;

  if ((V = malloc(sizeof(bz6VERTEX) * SplitW * SplitH)) == NULL)
    return;

  for (i = 0, tetha = 0; i < SplitH; i++, tetha += 2 * PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      V[j + SplitW * i].P = VecSet(C.X + (r * sin(phi) + R + r) * sin(tetha),
                                       C.Y + r * cos(phi),
                                       C.Z + (r * sin(phi) + R + r) * cos(tetha));

  
  BZ6_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
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
  INT i, nv = 0, nind = 0, size;
  bz6VERTEX *V;
  INT *Ind;
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

  size = sizeof(bz6VERTEX) * nv + sizeof(INT) * nind;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);
  memset(V, 0, size);

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
      V[nv].N = VecSet(0, 0, 0);
      V[nv++].P = VecSet(x, y, z);
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
            Ind[nind++] = n0;
            Ind[nind++] = n1;
            Ind[nind++] = nc;
            n1 = nc;
          }
          n++;
        }
    }
  }
  fclose(F);

  for (i = 0; i < nind; i+= 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

      V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
      V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
      V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }
  
  for (i = 0; i < nv; i++)
  {
    VEC L = {1, 1, 1};
    FLT nl = VecDotVec(VecNormalize(V[i].N), L);
    
    if (nl < 0.1)
      nl = 0.1;
    V[i].C = Vec4Set(0.3 * nl, 0.2 * nl, 0.1 * nl, 1);
  }
  BZ6_RndPrimCreate(Pr, V, nv, Ind, nind);
  free(V);
  return TRUE;
} /* End of 'BZ6_RndPrimLoad' function */


/* END OF 'rndprim.c' FILE */