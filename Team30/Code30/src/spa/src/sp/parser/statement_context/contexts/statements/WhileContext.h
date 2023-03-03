#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class WhileContext : public RecursiveParseContext {
 public:
  explicit WhileContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
