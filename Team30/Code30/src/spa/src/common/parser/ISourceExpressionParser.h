#pragma once

#include <string>
#include "common/ast/AST.h"

using std::string;

class ISourceExpressionParser {
 public:
  virtual ~ISourceExpressionParser() = default;
  virtual AST parseExpression(string expression) = 0;
};
