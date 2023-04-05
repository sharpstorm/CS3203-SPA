#pragma once

#include "sp/ast/ASTNode.h"
#include "../ProcedureContextType.h"
#include "../IProcedureContextProvider.h"

class StatementContext : public RecursiveProcedureParseContext {
 public:
  explicit StatementContext(IProcedureContextProvider *provider) :
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;

 private:
  template<ProcedureContextType CONTEXT_TYPE>
  ASTNodePtr invokeSubcontext(SourceParseState *state) const;
};
