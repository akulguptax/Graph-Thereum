#include "utils/utils.h"
#include <iostream>

using std::string;
using std::pair;

void print_elapsed(clock_t& c1, clock_t& c2, const std::string_view& action) {
  double elapsed = ((double) (c2 - c1)) / CLOCKS_PER_SEC;
  std::cout << CYAN << "Elapsed CPU time for " << action << " was " << elapsed
            << " seconds." << RESET << std::endl;
}

bool compare_bc_pair(const pair<string, double>& p1, const pair<string, double>& p2) {
  return p1.second > p2.second;
}
