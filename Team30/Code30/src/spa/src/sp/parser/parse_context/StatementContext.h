#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

using std::shared_ptr;

class StatementContext: public RecursiveParseContext {
 public:
  StatementContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<ASTNode> generateAssign(SourceParseState* state);
};
