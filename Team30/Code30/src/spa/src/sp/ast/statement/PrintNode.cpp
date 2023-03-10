#include "PrintNode.h"

#include "sp/ast/ASTNode.h"

PrintNode::PrintNode(int lineNumber) :
    StatementASTNode(ASTNODE_PRINT, "", lineNumber) {
  children.push_back(nullptr);
}

void PrintNode::accept(IExtractor* e) {
  e->visitPrint(this);
}

string PrintNode::toString() {
  return ":print";
}
