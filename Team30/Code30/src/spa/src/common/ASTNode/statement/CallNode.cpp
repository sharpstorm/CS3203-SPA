#include <string>
#include <vector>
#include <memory>
#include "CallNode.h"

using std::vector, std::shared_ptr, std::string;

CallNode::CallNode(string pName) : StatementASTNode(ASTNODE_CALL, ""), procName(pName) {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void CallNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void CallNode::leave(shared_ptr<Extractor> e) {}

string CallNode::toString() {
  return ":call";
}
