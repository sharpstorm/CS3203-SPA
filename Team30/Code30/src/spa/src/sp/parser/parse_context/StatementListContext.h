#pragma once

#include <memory>
#include <vector>
#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

using std::shared_ptr;

class StatementListContext: public RecursiveParseContext {
 public:
  explicit StatementListContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
