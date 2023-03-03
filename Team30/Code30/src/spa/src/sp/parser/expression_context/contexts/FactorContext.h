#pragma once

#include "../ExpressionContextType.h"
#include "../../entity_context/IEntityParser.h"

class FactorContext: public RecursiveExpressionParseContext {
 public:
  explicit FactorContext(IExpressionContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  IEntityParserPtr entityParser;
};
