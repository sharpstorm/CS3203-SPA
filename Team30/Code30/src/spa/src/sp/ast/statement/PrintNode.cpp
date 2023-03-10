#include "PrintNode.h"

#include <vector>
#include "sp/ast/ASTNode.h"

using std::vector, std::shared_ptr;

PrintNode::PrintNode(int lineNumber) :
    StatementASTNode(ASTNODE_PRINT, "", lineNumber) {
  children = vector<ASTNodePtr>{nullptr};
}

void PrintNode::accept(IExtractor* e) {
  e->visitPrint(this);
}

string PrintNode::toString() {
  return ":print";
}
