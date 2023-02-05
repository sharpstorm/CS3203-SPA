#include <vector>
#include <memory>
#include "ReadNode.h"

using std::vector, std::shared_ptr;

ReadNode::ReadNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void ReadNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string ReadNode::toString() {
  return "Print: " + children[0]->toString();
}
