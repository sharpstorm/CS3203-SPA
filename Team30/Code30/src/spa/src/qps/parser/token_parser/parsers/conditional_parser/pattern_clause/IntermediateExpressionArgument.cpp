#include "IntermediateExpressionArgument.h"

IntermediateExpressionArgument::IntermediateExpressionArgument(
    const PatternString &literalPattern, const bool isPartial) :
    literalPattern(literalPattern), isPartial(isPartial) {}

bool IntermediateExpressionArgument::isWildcard() {
  return literalPattern.empty() && isPartial;
}

bool IntermediateExpressionArgument::allowsPartial() {
  return isPartial;
}

IASTPtr IntermediateExpressionArgument::parse(
    ISourceExpressionParser *parser) {
  return parser->parseExpression(literalPattern);
}
