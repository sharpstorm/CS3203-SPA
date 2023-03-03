#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ast/ASTNode.h"

class StatementContext: public RecursiveParseContext {
 public:
  explicit StatementContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
