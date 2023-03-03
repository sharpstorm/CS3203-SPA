#pragma once

#include "../ExpressionContextType.h"
#include "../../entity_context/IEntityParser.h"

class FactorContext: public RecursiveExpressionParseContext {
 public:
  explicit FactorContext(IExpressionContextProvider* provider,
                IEntityParser* entityParser):
      RecursiveParseContext(provider),
      entityParser(entityParser) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 private:
  IEntityParser* entityParser;
};
