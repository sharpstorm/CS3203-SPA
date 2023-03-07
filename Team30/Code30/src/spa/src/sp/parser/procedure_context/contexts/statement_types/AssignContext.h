#pragma once

#include "sp/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

class AssignContext : public RecursiveProcedureParseContext {
 public:
  explicit AssignContext(IProcedureContextProvider* provider):
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state) override;
};
