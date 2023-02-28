#pragma once

#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class CallContext : public RecursiveParseContext {
 public:
  explicit CallContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
