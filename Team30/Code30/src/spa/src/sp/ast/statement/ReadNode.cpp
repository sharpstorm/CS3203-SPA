#include <vector>

#include "ReadNode.h"
#include "sp/ast/ASTNode.h"

using std::vector;

ReadNode::ReadNode(int lineNumber) :
    StatementASTNode(ASTNODE_READ, "", lineNumber) {
  children = vector<ASTNodePtr>{nullptr};
}

void ReadNode::accept(Extractor* e) {
  e->visit(this);
}

string ReadNode::toString() {
  return ":read";
}
