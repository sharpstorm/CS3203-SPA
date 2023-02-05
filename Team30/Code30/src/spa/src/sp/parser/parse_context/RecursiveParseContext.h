#pragma once

#include "SourceParseContext.h"
#include "../IGrammarContextProvider.h"

class RecursiveParseContext: public SourceParseContext {
 protected:
  IGrammarContextProvider* contextProvider;
  explicit RecursiveParseContext(IGrammarContextProvider* contextProvider):
      contextProvider(contextProvider) {}
};
