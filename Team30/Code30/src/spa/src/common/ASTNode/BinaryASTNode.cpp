#include <vector>
#include "BinaryASTNode.h"

using std::vector;

BinaryASTNode::BinaryASTNode(ASTNodeType type, string value)
    : ASTNode(type, value) {
  children = vector<ASTNodePtr>{ nullptr, nullptr };
}

BinaryASTNode::BinaryASTNode(ASTNodeType type): ASTNode(type) {}

void BinaryASTNode::setLeftChild(ASTNodePtr left) {
  children[0] = left;
}

void BinaryASTNode::setRightChild(ASTNodePtr right) {
  children[1] = right;
}

ASTNodePtr BinaryASTNode::getLeftChild() {
  return children[0];
}

ASTNodePtr BinaryASTNode::getRightChild() {
  return children[1];
}
