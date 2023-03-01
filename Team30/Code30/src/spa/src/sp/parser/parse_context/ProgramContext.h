#pragma once

#include "RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class ProgramContext : public RecursiveParseContext {
 public:
  explicit ProgramContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
