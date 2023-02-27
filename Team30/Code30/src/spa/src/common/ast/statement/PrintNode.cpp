#include "PrintNode.h"

#include <vector>
#include "../ASTNode.h"

using std::vector, std::shared_ptr;

PrintNode::PrintNode(int lineNumber) :
    StatementASTNode(ASTNODE_PRINT, "", lineNumber) {
  children = vector<ASTNodePtr>{nullptr};
}

void PrintNode::accept(Extractor* e) {
  e->visit(this);
}

string PrintNode::toString() {
  return ":print";
}
