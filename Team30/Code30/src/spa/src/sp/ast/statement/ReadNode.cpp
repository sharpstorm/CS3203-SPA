#include "ReadNode.h"

ReadNode::ReadNode(LineNumber line) :
    StatementASTNode(ASTNODE_READ, "", line) {
  children.push_back(nullptr);
}

void ReadNode::accept(IExtractor* e) const {
  e->visitRead(this);
}
