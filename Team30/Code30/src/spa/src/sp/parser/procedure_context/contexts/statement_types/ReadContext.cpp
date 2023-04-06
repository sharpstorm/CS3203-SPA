#include "ReadContext.h"

#include <memory>
#include <utility>

using std::make_unique;

ASTNodePtr ReadContext::makeNode(const LineNumber lineNumber,
                                 ASTNodePtr variableNode) const {
  ASTNodePtr newNode = make_unique<ReadNode>(lineNumber);
  newNode->setChild(0, std::move(variableNode));
  return newNode;
}
