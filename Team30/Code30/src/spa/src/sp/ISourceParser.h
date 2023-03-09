#pragma once

#include <string>
#include "sp/ast/AST.h"

using std::string;

class ISourceParser {
 public:
  virtual ASTPtr parseSource(string input) = 0;
  virtual ASTPtr parseExpression(string expr) = 0;
};
