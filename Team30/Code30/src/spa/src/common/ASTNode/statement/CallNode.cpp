#include <string>
#include <vector>
#include <memory>
#include "CallNode.h"

using std::vector, std::shared_ptr, std::string;

CallNode::CallNode() : StatementASTNode(ASTNODE_PRINT, "") {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void CallNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void CallNode::leave(shared_ptr<Extractor> e) {}

string CallNode::toString() {
  return ":print";
}
