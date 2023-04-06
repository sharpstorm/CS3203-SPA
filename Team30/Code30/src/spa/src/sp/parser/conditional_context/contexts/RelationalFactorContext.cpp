#include "RelationalFactorContext.h"

RelationalFactorContext::RelationalFactorContext(
    IExpressionParser *exprParser) : exprParser(exprParser) {}

ASTNodePtr RelationalFactorContext::generateSubtree(SourceParseState *state)
const {
  return exprParser->parse(state);
}
