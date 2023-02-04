#include "AbstractBinaryASTNode.h"

using std::make_unique;

AbstractBinaryASTNode::AbstractBinaryASTNode() {
  children = vector<shared_ptr<ASTNode>>{ nullptr, nullptr };
}

void AbstractBinaryASTNode::setLeftChild(shared_ptr<ASTNode> left) {
  children[0] = left;
}

void AbstractBinaryASTNode::setRightChild(shared_ptr<ASTNode> right) {
  children[1] = right;
}

shared_ptr<ASTNode> AbstractBinaryASTNode::getLeftChild() {
  return children[0];
}

shared_ptr<ASTNode> AbstractBinaryASTNode::getRightChild() {
  return children[1];
}
