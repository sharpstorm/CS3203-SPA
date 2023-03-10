#include "BinaryASTNode.h"

#include <vector>
#include <utility>

using std::vector;

BinaryASTNode::BinaryASTNode(ASTNodeType type, string value)
    : ASTNode(type, value) {
  children.push_back(nullptr);
  children.push_back(nullptr);
}

BinaryASTNode::BinaryASTNode(ASTNodeType type): ASTNode(type) {
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void BinaryASTNode::setLeftChild(ASTNodePtr left) {
  children[0] = std::move(left);
}

void BinaryASTNode::setRightChild(ASTNodePtr right) {
  children[1] = std::move(right);
}

ASTNode* BinaryASTNode::getLeftChild() {
  return children[0].get();
}

ASTNode* BinaryASTNode::getRightChild() {
  return children[1].get();
}
