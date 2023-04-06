#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/ast/statement/CallNode.h"
#include "AbstractSingleArgContext.h"

class CallContext :
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_CALL> {
 public:
  explicit CallContext(IProcedureContextProvider *provider) :
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const LineNumber lineNumber,
                      ASTNodePtr variableNode) const override;
};
