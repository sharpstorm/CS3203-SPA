#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ast/ASTNode.h"

class StatementListContext: public RecursiveParseContext {
 public:
  explicit StatementListContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
