#pragma once

#include "sp/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "sp/ast/statement/ReadNode.h"

class ReadContext :
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_READ> {
 public:
  explicit ReadContext(IProcedureContextProvider *provider) :
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const LineNumber lineNumber,
                      ASTNodePtr variableNode) const override;
};
