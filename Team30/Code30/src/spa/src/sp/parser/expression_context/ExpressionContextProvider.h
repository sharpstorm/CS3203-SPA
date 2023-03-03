#pragma once

#include "../SourceParseContext.h"
#include "../IGrammarContextProvider.h"
#include "../IContextProvider.h"
#include "ExpressionContextType.h"

#include "contexts/FactorContext.h"
#include "contexts/ExpressionContext.h"
#include "contexts/TermContext.h"

class ExpressionContextProvider : IExpressionContextProvider {
 public:
  SourceParseContext* getContext(ExpressionContextType type);

 private:
  ExpressionContext exprContext;
  FactorContext factorContext;
  TermContext termContext;
};
