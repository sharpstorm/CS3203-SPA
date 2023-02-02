#pragma once

#include <memory>
#include "IPQLContext.h"

using std::unique_ptr;

class PQLGrammarContextProvider {
 public:
  unique_ptr<IPQLContext> getContext(PQLToken* token);
};
