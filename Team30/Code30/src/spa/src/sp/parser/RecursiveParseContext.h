#pragma once

#include "SourceParseContext.h"
#include "IGrammarContextProvider.h"

template <class IContextProvider>
class RecursiveParseContext: public SourceParseContext {
 protected:
  IContextProvider* contextProvider;
  explicit RecursiveParseContext(IContextProvider* contextProvider):
      contextProvider(contextProvider) {}
  }
};
