#pragma once

#include "../RecursiveParseContext.h"
#include "../IContextProvider.h"

enum class ExpressionContextType {
  EXPR_CONTEXT,
  TERM_CONTEXT,
  FACTOR_CONTEXT,
};

typedef IContextProvider<ExpressionContextType> IExpressionContextProvider;
typedef RecursiveParseContext<IExpressionContextProvider>
    RecursiveExpressionParseContext;
