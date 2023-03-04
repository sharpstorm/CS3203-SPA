#pragma once

#include <string>
#include <memory>

#include "common/pattern/ExpressionSequence.h"

class ExpressionArgument {
 public:
  ExpressionArgument(string literalPattern, bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  string getPattern();

 private:
  string literalPattern;
  bool isPartial;
};

typedef unique_ptr<ExpressionArgument> ExpressionArgumentPtr;
