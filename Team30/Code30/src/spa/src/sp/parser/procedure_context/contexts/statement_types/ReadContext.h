#pragma once

#include <memory>
#include <utility>

#include "sp/ast/ASTNode.h"
#include "AbstractSingleArgContext.h"
#include "sp/ast/statement/ReadNode.h"

using std::make_unique;

class ReadContext:
    public AbstractSingleArgContext<SIMPLE_TOKEN_KEYWORD_READ> {
 public:
  explicit ReadContext(IProcedureContextProvider* provider):
      AbstractSingleArgContext(provider) {}
 protected:
  ASTNodePtr makeNode(const int &lineNumber, ASTNodePtr variableNode) {
    ASTNodePtr newNode = make_unique<ReadNode>(lineNumber);
    newNode->setChild(0, std::move(variableNode));
    return std::move(newNode);
  }
};
