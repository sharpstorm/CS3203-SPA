#include <vector>

#include "ReadNode.h"
#include "sp/ast/ASTNode.h"

using std::vector;

ReadNode::ReadNode(int lineNumber) :
    StatementASTNode(ASTNODE_READ, "", lineNumber) {
  children.push_back(nullptr);
}

void ReadNode::accept(IExtractor* e) {
  e->visitRead(this);
}

string ReadNode::toString() {
  return ":read";
}
