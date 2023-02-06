#pragma once
#include <string>
#include "pkb/writers/PkbWriter.h"

using std::string;

class SpDriver {
 public:
  void parseSource(string input, PkbWriter* pkbWriter);
};
