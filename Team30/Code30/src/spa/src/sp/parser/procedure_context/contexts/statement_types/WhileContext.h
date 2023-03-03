#pragma once

#include "common/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"


class WhileContext : public RecursiveProcedureParseContext {
 public:
  explicit WhileContext(IProcedureContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
