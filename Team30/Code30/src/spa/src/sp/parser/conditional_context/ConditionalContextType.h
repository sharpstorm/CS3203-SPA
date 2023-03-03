#pragma once

#include "../IContextProvider.h"
#include "../RecursiveParseContext.h"

enum ConditionalContextType {
  COND_CONTEXT,
  REL_CONTEXT,
  REL_FACTOR_CONTEXT,
};

typedef IContextProvider<ConditionalContextType> IConditionalContextProvider;
typedef RecursiveParseContext<IConditionalContextProvider>
    RecursiveConditionalParseContext;
