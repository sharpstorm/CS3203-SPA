#pragma once

#include <memory>
#include "common/ast/ASTNode.h"
#include "common/ast/statement/CallNode.h"
#include "AbstractSingleArgContext.h"

using std::make_shared;

class CallContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_CALL> {
 public:
  explicit CallContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    return make_shared<CallNode>(lineNumber, variableNode->getValue());
  }
};
