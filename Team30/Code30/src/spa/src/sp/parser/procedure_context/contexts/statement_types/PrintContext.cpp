#include "PrintContext.h"

#include <memory>
#include <utility>

using std::make_unique;

ASTNodePtr PrintContext::makeNode(const LineNumber lineNumber,
                                  ASTNodePtr variableNode) const {
  ASTNodePtr newNode = make_unique<PrintNode>(lineNumber);
  newNode->setChild(0, std::move(variableNode));
  return newNode;
}
