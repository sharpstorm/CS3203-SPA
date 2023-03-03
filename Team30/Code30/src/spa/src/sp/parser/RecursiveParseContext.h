#pragma once

#include "SourceParseContext.h"

template <class IContextProvider>
class RecursiveParseContext: public SourceParseContext {
 protected:
  IContextProvider* contextProvider;
  explicit RecursiveParseContext(IContextProvider* contextProvider):
      contextProvider(contextProvider) {}
};
