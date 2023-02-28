#pragma once

#include <string>
#include "common/ast/AST.h"

using std::string;

class ISpExpressionParser {
 public:
  virtual ~ISpExpressionParser() = default;
  virtual AST parseExpression(string expression) = 0;
};
