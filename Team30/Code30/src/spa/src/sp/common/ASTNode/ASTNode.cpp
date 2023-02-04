#include "ASTNode.h"

vector<shared_ptr<ASTNode>> ASTNode::getChildren() {
  return children;
}

void ASTNode::setChild(int index, shared_ptr<ASTNode> node) {
  children[index] = node;
}