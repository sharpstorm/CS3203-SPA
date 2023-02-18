#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode(int lineNumber) :
    StatementASTNode(ASTNODE_PRINT, "", lineNumber) {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void PrintNode::accept(Extractor* e) {
  e->visit(this);
}

string PrintNode::toString() {
  return ":print";
}
