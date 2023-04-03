#include <vector>

#include "ReadNode.h"
#include "sp/ast/ASTNode.h"

using std::vector;

ReadNode::ReadNode(LineNumber line) :
    StatementASTNode(ASTNODE_READ, "", line) {
  children.push_back(nullptr);
}

void ReadNode::accept(IExtractor* e) {
  e->visitRead(this);
}
