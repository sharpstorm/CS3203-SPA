#pragma once

#include <string>
#include "common/ast/AST.h"

using std::string;

class ISourceParser {
 public:
  virtual AST parseSource(string input) = 0;
};
