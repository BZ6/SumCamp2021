/* FILE: T01PIF.cpp
 * PROGRAMMER: BZ6
 * DATE: 18.07.2021
 * PURPOSE: Pifagor table.
 */
#include <iostream>

int main( void )
{
  float FirstNumber, StepSize;
  int SizeOfTable;

  std::cout << "Input first number:";
  std::cin >> FirstNumber;
  std::cout << "Input STEP SIZE:";
  std::cin >> StepSize;
  std::cout << "Input size of table:";
  std::cin >> SizeOfTable;

  float k;

  for (int i = 0; i < SizeOfTable; i++, std::cout << "\n")
    for (int j = 0; j < SizeOfTable; j++, std::cout << " ")
    {
      std::cout.width(8);
      std::cout.precision(3);
      std::cout << std::fixed << (FirstNumber + i * StepSize) * (FirstNumber + j * StepSize);
    }    
}

/* END OF 'T01PIF.cpp' FILE*/