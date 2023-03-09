#pragma once

#include <memory>
#include <utility>

#include "sp/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "sp/ast/statement/PrintNode.h"

using std::make_unique;

class PrintContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_PRINT> {
 public:
  explicit PrintContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    ASTNodePtr newNode = make_unique<PrintNode>(lineNumber);
    newNode->setChild(0, std::move(variableNode));
    return std::move(newNode);
  }
};
