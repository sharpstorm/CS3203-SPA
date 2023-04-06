#pragma once

#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

class IfContext : public RecursiveProcedureParseContext {
 public:
  explicit IfContext(IProcedureContextProvider *provider) :
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;
};
