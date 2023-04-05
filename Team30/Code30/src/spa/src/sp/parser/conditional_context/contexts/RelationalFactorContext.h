#pragma once

#include "../ConditionalContextType.h"
#include "../../expression_context/ExpressionParser.h"

class RelationalFactorContext: public SourceParseContext {
 public:
  explicit RelationalFactorContext(IExpressionParser* exprParser);
  ASTNodePtr generateSubtree(SourceParseState* state) const override;

 private:
  IExpressionParser* exprParser;
};
