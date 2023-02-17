#include "ASTNode.h"

vector<shared_ptr<ASTNode>> ASTNode::getChildren() {
  return children;
}

void ASTNode::setChild(int index, shared_ptr<ASTNode> node) {
  children[index] = node;
}

void ASTNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

shared_ptr<IASTNode> ASTNode::getChild (int index) {
  return children[index];
}

ASTNodeType ASTNode::getType() {
  return type;
}

