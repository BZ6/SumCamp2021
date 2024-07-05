/* FILE NAME: t04perm.c
 * PROGRAMMER: BZ6
 * DATE: 12.06.2021
 * PURPOSE: Inversion.
 */

#include <stdio.h>
#include <windows.h>

#define MAX 4
INT p[MAX];
BOOL IsParity = TRUE;



VOID Store( VOID )
{
  FILE *F;
  INT i;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < MAX - 1; i++)
    fprintf(F, "%d, ", p[i]);
  fprintf(F, "%d - parity: %s\n", p[i], IsParity ? "even" : "odd");
  fclose(F);
}

VOID Fill( VOID )
{
  INT i;

  for (i = 0; i < MAX; i++)
    p[i] = i + 1;
}

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
  IsParity = !IsParity;
}

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
  BOOL sParity;

  if (n == MAX)
    Store();
  else
  {
    sParity = IsParity;
    Go(n + 1);
    for (i = n + 1; i < MAX; i++)
    {
      Swap(&p[n] , &p[i]);
      Go(n + 1);
    }
    x = p[n];
    for (i = n + 1; i < MAX; i++)
      p[i - 1] = p[i];
    p[MAX - 1] = x;
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
  Fill();
  Go(0);
} /* End of 'main' function */

/* END OF 't04perm.c' FILE */