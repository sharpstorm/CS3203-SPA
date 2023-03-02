#include "ExpressionArgument.h"

ExpressionArgument::ExpressionArgument(string literalPattern, bool isPartial):
    literalPattern(literalPattern), isPartial(isPartial) {}

bool ExpressionArgument::isWildcard() {
  return literalPattern.empty() && isPartial;
}

bool ExpressionArgument::allowsPartial() {
  return isPartial;
}

string ExpressionArgument::getPattern() {
  return literalPattern;
}
