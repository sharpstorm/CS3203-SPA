#pragma once

#include <string>
#include <memory>
#include <vector>

#include "common/pattern/ExpressionSequence.h"

class ExpressionArgument {
 public:
  ExpressionArgument();
  ExpressionArgument(ExpressionSequencePtr expression, bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  ExpressionSequence* getSequence();

 private:
  ExpressionSequencePtr expressionSequence;
  bool isPartial;
};

typedef unique_ptr<ExpressionArgument> ExpressionArgumentPtr;
