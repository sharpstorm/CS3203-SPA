#pragma once

#include <memory>
#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

using std::shared_ptr;

class StatementListContext: public RecursiveParseContext {
 public:
  explicit StatementListContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<ASTNode> generateAssign(SourceParseState* state);
};
