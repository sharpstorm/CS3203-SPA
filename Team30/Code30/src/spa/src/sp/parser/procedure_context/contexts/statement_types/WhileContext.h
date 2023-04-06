#pragma once

#include "sp/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

class WhileContext : public RecursiveProcedureParseContext {
 public:
  explicit WhileContext(IProcedureContextProvider *provider) :
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;
};
