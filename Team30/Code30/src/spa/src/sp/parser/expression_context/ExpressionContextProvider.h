#pragma once

#include "../SourceParseContext.h"
#include "../IContextProvider.h"
#include "ExpressionContextType.h"

#include "contexts/FactorContext.h"
#include "contexts/ExpressionContext.h"
#include "contexts/TermContext.h"

class ExpressionContextProvider : public IExpressionContextProvider {
 public:
  explicit ExpressionContextProvider(IEntityParser *entityParser);
  ASTNodePtr generateSubtree(ExpressionContextType type,
                             SourceParseState *state) const override;

 private:
  ExpressionContext exprContext;
  FactorContext factorContext;
  TermContext termContext;

  const SourceParseContext *getContext(ExpressionContextType type) const;
};
