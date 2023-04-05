#pragma once

#include "sp/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "sp/ast/statement/PrintNode.h"

class PrintContext :
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_PRINT> {
 public:
  explicit PrintContext(IProcedureContextProvider *provider) :
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const LineNumber lineNumber,
                      ASTNodePtr variableNode) const override;
};
