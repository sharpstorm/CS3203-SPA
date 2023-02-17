#include <vector>
#include <memory>
#include "BinaryASTNode.h"

using std::vector, std::shared_ptr;

BinaryASTNode::BinaryASTNode() {
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

void BinaryASTNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string BinaryASTNode::toString() {
  return ":operand";
};
