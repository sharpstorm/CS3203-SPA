#pragma once

#include "../SourceParseContext.h"
#include "../IContextProvider.h"
#include "ExpressionContextType.h"

#include "contexts/FactorContext.h"
#include "contexts/ExpressionContext.h"
#include "contexts/TermContext.h"

class ExpressionContextProvider : public IExpressionContextProvider {
 public:
  explicit ExpressionContextProvider(IEntityParser* entityParser);
  SourceParseContext* getContext(ExpressionContextType type);

 private:
  ExpressionContext exprContext;
  FactorContext factorContext;
  TermContext termContext;
};
