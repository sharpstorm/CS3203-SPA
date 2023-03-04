#include "RelationalFactorContext.h"

RelationalFactorContext::RelationalFactorContext(
    IExpressionParser *exprParser):exprParser(exprParser) {}

ASTNodePtr RelationalFactorContext::generateSubtree(SourceParseState *state) {
  return exprParser->parse(state);
}
