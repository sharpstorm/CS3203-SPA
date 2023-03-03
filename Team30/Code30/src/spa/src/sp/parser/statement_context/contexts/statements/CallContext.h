#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class CallContext : public RecursiveParseContext {
 public:
  explicit CallContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
