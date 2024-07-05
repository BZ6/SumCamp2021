/* FILE NAME: t04perm.c
 * PROGRAMMER: BZ6
 * DATE: 12.06.2021
 * PURPOSE: Determinant.
 */

#include <stdio.h>
#include <windows.h>

typedef DOUBLE DBL;

#define MAX 5
DBL A[MAX][MAX], Det = 0; 
INT p[MAX], N;
BOOL IsParity = TRUE;

BOOL LoadMatrix( CHAR *FileName );
VOID Store( CHAR *FileName );
VOID Fill( VOID );
VOID Swap( INT *A, INT *B );
VOID Go( INT n );

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
}

/* The output function.
 * ARGUMENTS:
 *      VOID;
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

/* The fill massive function.
 * ARGUMENTS:
 *      VOID;
 * RETURNS:
 *      VOID.
 */
VOID Fill( VOID )
{
  INT i;

  for (i = 0; i < N; i++)
    p[i] = i;
} /* End of 'Fill' function */

/* The swap function.
 * ARGUMENTS:
 *   - 2 numbers for swap:
 *      INT *A, *B;
 * RETURNS:
 *      VOID.
 */
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
  IsParity = !IsParity;
} /* End of 'Swap' function */

/* The permutation program function.
 * ARGUMENTS:
 *   - Position in number:
 *      INT n;
 * RETURNS:
 *      VOID.
 */
VOID Go( INT n )
{
  INT i, x;
  DBL prod = 1;
  BOOL sParity;

  if (n == N)
  {
    for (i = 0; i < N; i++)
      prod *= A[i][p[i]];
    Det += (IsParity * 2 - 1) * prod;
  }
  else
  {
    sParity = IsParity;
    Go(n + 1);
    for (i = n + 1; i < N; i++)
    {
      Swap(&p[n] , &p[i]);
      Go(n + 1);
    }
    x = p[n];
    for (i = n + 1; i < N; i++)
      p[i - 1] = p[i];
    p[N - 1] = x;
    IsParity = sParity;
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
  Fill();
  Go(0);
  Store("IN.txt");
} /* End of 'main' function */

/* END OF 't05det.c' FILE */