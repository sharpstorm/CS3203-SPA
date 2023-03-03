#pragma once

#include "common/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

class AssignContext : public RecursiveProcedureParseContext {
 public:
  AssignContext(IProcedureContextProvider* provider):
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
