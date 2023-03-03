#pragma once

#include <memory>
#include "common/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "common/ast/statement/PrintNode.h"

using std::make_shared;

class PrintContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_PRINT> {
 public:
  PrintContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    ASTNodePtr newNode = make_shared<PrintNode>(lineNumber);
    newNode->setChild(0, variableNode);
    return newNode;
  }
};
