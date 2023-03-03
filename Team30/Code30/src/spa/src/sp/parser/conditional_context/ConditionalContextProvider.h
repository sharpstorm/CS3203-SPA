#pragma once

#include "../IContextProvider.h"
#include "ConditionalContextType.h"

#include "contexts/ConditionalExpressionContext.h"
#include "contexts/RelationalExpressionContext.h"
#include "contexts/RelationalFactorContext.h"

class ConditionalContextProvider : public IConditionalContextProvider {
 public:
  explicit ConditionalContextProvider(IExpressionParser* exprParser);
  SourceParseContext* getContext(ConditionalContextType type);

 private:
  ConditionalExpressionContext condContext;
  RelationalExpressionContext relContext;
  RelationalFactorContext relFactorContext;
};
