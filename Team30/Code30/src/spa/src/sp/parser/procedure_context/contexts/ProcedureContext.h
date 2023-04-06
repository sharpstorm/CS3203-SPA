#pragma once

#include "sp/ast/ASTNode.h"
#include "../ProcedureContextType.h"
#include "../IProcedureContextProvider.h"

class ProcedureContext : public RecursiveProcedureParseContext {
 public:
  explicit ProcedureContext(IProcedureContextProvider *provider) :
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;
};
