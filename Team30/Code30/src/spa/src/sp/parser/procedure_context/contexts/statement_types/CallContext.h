#pragma once

#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/ast/statement/CallNode.h"
#include "AbstractSingleArgContext.h"

using std::make_unique;

class CallContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_CALL> {
 public:
  explicit CallContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    return make_unique<CallNode>(lineNumber, variableNode->getValue());
  }
};
