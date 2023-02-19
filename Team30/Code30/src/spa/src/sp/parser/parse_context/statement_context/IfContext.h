#pragma once

#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ASTNode/ASTNode.h"

class IfContext : public RecursiveParseContext {
 public:
  explicit IfContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
