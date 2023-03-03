#pragma once

#include <memory>
#include "common/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "common/ast/statement/ReadNode.h"

using std::make_shared;

class ReadContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_READ> {
 public:
  ReadContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    ASTNodePtr newNode = make_shared<ReadNode>(lineNumber);
    newNode->setChild(0, variableNode);
    return newNode;
  }
};
