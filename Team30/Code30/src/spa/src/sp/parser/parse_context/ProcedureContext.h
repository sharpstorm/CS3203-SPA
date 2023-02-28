#pragma once

#include "RecursiveParseContext.h"
#include "common/ast/ASTNode.h"

class ProcedureContext : public RecursiveParseContext {
 public:
  explicit ProcedureContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
