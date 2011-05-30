#include "io.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string IO::readFile(const std::string& filename) {
  std::ifstream ifs (filename.c_str(), std::ifstream::in );
  std::stringstream output;

  while (ifs.good()) {
    output << (char) ifs.get();
  }
  return output.str();
}

