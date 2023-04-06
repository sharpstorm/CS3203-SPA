#include "CallContext.h"

#include <memory>

using std::make_unique;

ASTNodePtr CallContext::makeNode(const LineNumber lineNumber,
                                 ASTNodePtr variableNode) const {
  return make_unique<CallNode>(lineNumber, variableNode->getValue());
}
