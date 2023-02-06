#pragma once
#include <string>
#include "pkb/writers/PkbWriter.h"

using std::string;

class ISp {
 public:
  virtual void parseSource(string input, PkbWriter* pkbWriter) = 0;
};
