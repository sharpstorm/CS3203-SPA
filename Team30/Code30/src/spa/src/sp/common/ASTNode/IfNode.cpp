#include <vector>
#include <memory>
#include "IfNode.h"

using std::vector, std::shared_ptr;

IfNode::IfNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr, nullptr};
}

void IfNode::accept() {}

void IfNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string IfNode::toString() {
  string ss = "If: ";
  ss += "\n";
  ss += "then: \n";
  ss += children[1]->toString();
  ss += "else: \n";
  ss += children[2]->toString();
  return ss;
}
