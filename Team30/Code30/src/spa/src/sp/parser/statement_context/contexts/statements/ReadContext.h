#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class ReadContext : public RecursiveParseContext {
 public:
  explicit ReadContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
