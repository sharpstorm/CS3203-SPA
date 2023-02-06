#pragma once

#include <string>
#include "sp/common/AST.h"

using std::string;

class ISourceParser {
 public:
  virtual AST parseSource(string input) = 0;
};
