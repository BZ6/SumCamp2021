/* FILE: T01SORT.cpp
 * PROGRAMMER: BZ6
 * DATE: 18.07.2021
 * PURPOSE: Sort numbers.
 */
#include <iostream>
#include <fstream>
#include <string>

void BubbleSort( int *p, int N );

int main(void)
{
  int N;
  int *p;
  std::string s;
  std::fstream f;
  
  std::cin >> s;

  f.open(s, std::fstream::in);
  f >> N;
  p = (int *)malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++)
    f >> p[i];
  f.close();

  BubbleSort(p, N);

  f.open("output.txt", std::fstream::out);
  for (int i = 0; i < N; i++)
    f << p[i] << "\n";
  f.close();
  if (p != nullptr)
    free(p);
}

void BubbleSort( int *p, int N )
{
  for (int i = 1; i < N; i++)
    for (int j = i; j < N; j++)
      if (p[j] < p[j - 1])
      {
        int tmp = p[j];
        p[j] = p[j - 1];
        p[j - 1] = tmp;
      }
}

/* END OF 'T01SORT.cpp' FILE*/