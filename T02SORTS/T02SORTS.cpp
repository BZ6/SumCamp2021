/* FILE: T02SORTS.cpp
 * PROGRAMMER: BZ6
 * DATE: 18.07.2021
 * PURPOSE: Sort numbers.
 */
#include <iostream>
#include <fstream>
#include <string>

void BubbleSort( int *p, int N );

struct array
{
  //Data
  int N, *p;

  array( void ) : p(nullptr), N(0)
  {
  }
  ~array( void )
  {
    if (p != nullptr)
      delete[] p, p = nullptr;
  }
  void load( const std::string &FileName = "input.txt" )
  {
    if (p != nullptr)
      delete[] p, N = 0, p = nullptr;
    std::fstream f;
    f.open(FileName, std::fstream::in);
    f >> N;
    p = new int[N];
    for (int i = 0; i < N; i++)
      f >> p[i];
    f.close();
  }
  void save( const std::string &FileName = "output.txt" )
  {
    std::fstream f;
    f.open(FileName, std::fstream::out);
    f << N << "\n";
    for (int i = 0; i < N; i++)
      f << p[i] << "\n";
    f.close();
  }
  void sort( void )
  {
    BubbleSort(p, N);
  }
};

int main( void )
{
  array a;
  //std::string s;
  
  //std::cout << "File name for input";
  //std::cin >> s;
  //if (s == "0")
  a.load();
  //else
  //  a.load(s);
  a.sort();
  //std::cout << "File name for output";
  //std::cin >> s;
  //if (s == "0")
  a.save();
  //else
  //  a.save(s);
}

void BubbleSort(int *p, int N )
{
  for (int i = 0; i < N - 1; i++)
    for (int j = 0; j < N - i - 1; j++)
      if (p[j] > p[j + 1])
      {
        int tmp = p[j];
        p[j] = p[j + 1];
        p[j + 1] = tmp;
      }
}

/* END OF 'T02SORTS.cpp' FILE*/