#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class AssignContext : public RecursiveParseContext {
 public:
  explicit AssignContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
