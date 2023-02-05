#include "ConstantASTNode.h"

ConstantASTNode::ConstantASTNode(string v) {
  value = v;
}

string ConstantASTNode::toString() {
  return "const(" + value + ")";
}

void ConstantASTNode::accept() {
}
