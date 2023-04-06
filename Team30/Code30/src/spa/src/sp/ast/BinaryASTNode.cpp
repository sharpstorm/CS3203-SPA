#include "BinaryASTNode.h"

#include <utility>

BinaryASTNode::BinaryASTNode(ASTNodeType type, const ASTNodeValue &value)
    : ASTNode(type, value) {
  children.push_back(nullptr);
  children.push_back(nullptr);
}

BinaryASTNode::BinaryASTNode(ASTNodeType type) : ASTNode(type) {
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void BinaryASTNode::setLeftChild(ASTNodePtr left) {
  children[0] = std::move(left);
}

void BinaryASTNode::setRightChild(ASTNodePtr right) {
  children[1] = std::move(right);
}
