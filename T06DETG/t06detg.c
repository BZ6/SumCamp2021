/* FILE NAME: t06detg.c
 * PROGRAMMER: BZ6
 * DATE: 14.06.2021
 * PURPOSE: Determinant.
 */

#include <windows.h>
#include <stdio.h>
#include <math.h>

typedef DOUBLE DBL;

/* Global */
#define MAX 5
DBL A[MAX][MAX], Det = 1; 
INT N;

/* Forward declaration */
BOOL LoadMatrix( CHAR *FileName );
VOID Store( CHAR *FileName );
VOID Swap( DBL *A, DBL *B );
VOID Go( VOID );

/* The input matrix function.
 * ARGUMENTS:
 *   - numbers:
 *       DBL *A, *B;
 * RETURNS:
 *      VOID.
 */
BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
} /* End of 'LoadMatrix' function */

/* The output determinant of matrix function.
 * ARGUMENTS:
 *   - numbers:
 *       DBL *A, *B;
 * RETURNS:
 *      VOID.
 */
VOID Store( CHAR *FileName )
{
  FILE *F;

  F = fopen(FileName, "a");
  if (F == NULL)
    return;
  fprintf(F, "Determinant of matrix: %f\n", Det);
  fclose(F);
} /* End of 'Store' function */

/* The calculate determinant of matrix function.
 * ARGUMENTS:
 *   - numbers:
 *       DBL *A, *B;
 * RETURNS:
 *      VOID.
 */
VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* The calculate determinant of matrix function function.
 * ARGUMENTS:
 *      VOID;
 * RETURNS:
 *      VOID.
 */
VOID Go( VOID )
{
  INT i, j, k, x, y, max_row, max_col;
  DBL kf;

  for (i = 0; i < N; i++)
  {
    /* Look for maximum matrix element */
    max_row = max_col = i;
    for (y = i; y < N; y++)    
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_col = x, max_row = y;
    /* Look for zero on diagonale of matrix */
    if (A[max_row][max_col] == 0)
    {
      Det = 0;
      break;
    }
    /* Swap rows */
    if (max_row != i)
    {
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_row][x]);
      Det = -Det;
    }
    /* Swap Columns */
    if (max_col != i)
    {
      for (y = 0; y < N; y++)
        Swap(&A[y][i], &A[y][max_col]);
      Det = -Det;
    }
    /* Subtrack from every row i row multiplied by A[j][i] / A[i][i] */
    for (j = i + 1; j < N; j++)    
    {
      kf = A[j][i] / A[i][i];
      A[j][i] = 0;
      for (k = i + 1; k < N; k++)
        A[j][k] -= A[i][k] * kf;
    }
    Det *= A[i][i];   
  }
} /* End of 'Go' function */

/* The main program function.
 * ARGUMENTS:
 *      VOID;
 * RETURNS:
 *      VOID.
 */
VOID main( VOID )
{
  LoadMatrix("IN.txt");
  Go();
  Store("IN.txt");
} /* End of 'main' function */

/* END OF 't06detg.c' FILE */
