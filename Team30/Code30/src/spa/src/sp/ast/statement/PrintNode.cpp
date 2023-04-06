#include "PrintNode.h"

#include "sp/ast/ASTNode.h"

PrintNode::PrintNode(LineNumber line) :
    StatementASTNode(ASTNODE_PRINT, "", line) {
  children.push_back(nullptr);
}

void PrintNode::accept(IExtractor* e) {
  e->visitPrint(this);
}
