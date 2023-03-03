#pragma once

#include "common/ast/ASTNode.h"
#include "../ProcedureContextType.h"
#include "../IProcedureContextProvider.h"

class StatementListContext: public RecursiveProcedureParseContext {
 public:
  explicit StatementListContext(IProcedureContextProvider* provider):
      RecursiveParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);
};
