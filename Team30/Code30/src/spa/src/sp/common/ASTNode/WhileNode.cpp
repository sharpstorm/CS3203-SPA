#include <vector>
#include "memory"
#include "WhileNode.h"

using std::vector, std::shared_ptr;

WhileNode::WhileNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void WhileNode::accept() {}

void WhileNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string WhileNode::toString() {
  string ss = "while: " + children[0]->toString() + " | Children: ";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
