#pragma once

#include <string>
#include "common/ast/IAST.h"

using std::string;

class ISourceExpressionParser {
 public:
  virtual ~ISourceExpressionParser() = default;
  virtual IASTPtr parseExpression(string expression) = 0;
};
