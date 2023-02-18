#include <vector>
#include <memory>
#include "BinaryASTNode.h"

using std::vector, std::shared_ptr;

BinaryASTNode::BinaryASTNode(ASTNodeType type, string value)
    : ASTNode(type, value) {
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr };
}

void BinaryASTNode::setLeftChild(shared_ptr<ASTNode> left) {
  children[0] = left;
}

void BinaryASTNode::setRightChild(shared_ptr<ASTNode> right) {
  children[1] = right;
}

shared_ptr<ASTNode> BinaryASTNode::getLeftChild() {
  return children[0];
}

shared_ptr<ASTNode> BinaryASTNode::getRightChild() {
  return children[1];
}

string BinaryASTNode::toString() {
  return ":operand";
}
