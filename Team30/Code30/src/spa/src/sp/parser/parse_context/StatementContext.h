#pragma once

#include <memory>
#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

using std::shared_ptr;

class StatementContext: public RecursiveParseContext {
 public:
  explicit StatementContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<ASTNode> generateAssign(SourceParseState* state);
};
