#pragma once

#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class PrintContext : public RecursiveParseContext {
 public:
  explicit PrintContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
