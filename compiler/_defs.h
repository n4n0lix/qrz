#pragma once
#include "_global.h"

#include <limits>
#include <iostream>

#define POWER_OF_TWO(x) (1 << (x))

#define INT_BIT_SIZE    sizeof(int64)
#define INT_RADIX       std::numeric_limits<int64>::radix
#define INT_MAX_VALUE   std::numeric_limits<int64>::max()
#define INT_MIN_VALUE   std::numeric_limits<int64>::min()

void inline print_debug__int() {
  std::cout << "int\n------------------------\n";
  std::cout << "bits:  " << INT_BIT_SIZE << "\n";
  std::cout << "radix: " << INT_RADIX << "\n";
  std::cout << "max:   " << INT_MAX_VALUE << "\n";
  std::cout << "min:   " << INT_MIN_VALUE << "\n";
}

void inline print_defs() {
  std::cout << "\n\t\tDEFS\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
  print_debug__int();
}