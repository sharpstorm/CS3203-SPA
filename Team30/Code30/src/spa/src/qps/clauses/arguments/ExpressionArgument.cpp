#include "ExpressionArgument.h"

#include <utility>

ExpressionArgument::ExpressionArgument() :
    isPartial(true), expressionSequence(nullptr) {}

ExpressionArgument::ExpressionArgument(ExpressionSequencePtr expression,
                                       bool isPartial) :
    expressionSequence(std::move(expression)), isPartial(isPartial) {}

bool ExpressionArgument::isWildcard() const {
  return expressionSequence == nullptr && isPartial;
}

bool ExpressionArgument::allowsPartial() const {
  return isPartial;
}

const ExpressionSequence *ExpressionArgument::getSequence() const {
  return expressionSequence.get();
}
