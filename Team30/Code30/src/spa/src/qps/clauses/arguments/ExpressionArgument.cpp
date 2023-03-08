#include "ExpressionArgument.h"

#include <utility>

ExpressionArgument::ExpressionArgument():
    isPartial(true), expressionSequence(nullptr) {}

ExpressionArgument::ExpressionArgument(ExpressionSequencePtr expression,
                                       bool isPartial):
    expressionSequence(std::move(expression)), isPartial(isPartial) {}

bool ExpressionArgument::isWildcard() {
  return expressionSequence == nullptr && isPartial;
}

const bool ExpressionArgument::allowsPartial() {
  return isPartial;
}

ExpressionSequence* ExpressionArgument::getSequence() {
  return expressionSequence.get();
}
