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

void ASTNode::setAllLine(int lineNumber) {
//  if (children.size() == 0) {
//    return;
//  }
//
//  for (shared_ptr<ASTNode> child : children) {
//    child->lineNumber = lineNumber;
//    child->setAllLine(lineNumber);
//  }
}


