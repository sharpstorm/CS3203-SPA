#pragma once

#include "common/ast/IAST.h"
#include "sp/SPTypes.h"

class ISourceExpressionParser {
 public:
  virtual ~ISourceExpressionParser() = default;
  virtual IASTPtr parseExpression(const SourceExpression &expression) = 0;
};
