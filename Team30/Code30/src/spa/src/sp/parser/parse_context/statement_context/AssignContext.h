#pragma once

#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ASTNode/ASTNode.h"

class AssignContext : public RecursiveParseContext {
 public:
  explicit AssignContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
