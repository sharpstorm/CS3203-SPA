#pragma once

#include <memory>
#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

using std::shared_ptr;

class RelationalFactorContext: public RecursiveParseContext {
 public:
  explicit RelationalFactorContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
