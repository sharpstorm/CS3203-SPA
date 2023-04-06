#pragma once

#include <memory>

#include "common/pattern/ExpressionSequence.h"

using std::unique_ptr;

class ExpressionArgument {
 public:
  ExpressionArgument();
  ExpressionArgument(ExpressionSequencePtr expression, bool isPartial);
  bool isWildcard() const;
  const bool allowsPartial() const;
  const ExpressionSequence *getSequence() const;

 private:
  ExpressionSequencePtr expressionSequence;
  bool isPartial;
};

typedef unique_ptr<ExpressionArgument> ExpressionArgumentPtr;
