#pragma once

#include <string>
#include "sp/ast/AST.h"

using std::string;

class ISourceParser {
 public:
  virtual AST parseSource(string input) = 0;
  virtual AST parseExpression(string expr) = 0;
};
