#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode() : StatementASTNode(ASTNODE_PRINT, "") {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void PrintNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void PrintNode::leave(shared_ptr<Extractor> e) {}

string PrintNode::toString() {
  return ":print";
}
