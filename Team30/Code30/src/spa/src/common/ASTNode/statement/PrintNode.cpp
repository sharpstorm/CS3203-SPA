#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
  type = ASTNODE_PRINT;
}

void PrintNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string PrintNode::toString() {
  return ":print";
}
