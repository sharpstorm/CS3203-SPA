#include "ASTNode.h"

ASTNode::ASTNode(ASTNodeType t) : type(t), value("") {
}

ASTNode::ASTNode(ASTNodeType t, string v) : type(t), value(v) {
}

vector<ASTNodePtr> ASTNode::getChildren() {
  return children;
}

void ASTNode::setChild(int index, ASTNodePtr node) {
  children[index] = node;
}

void ASTNode::addChild(ASTNodePtr node) {
  children.push_back(node);
}

shared_ptr<IASTNode> ASTNode::getChild(int index) {
  return children[index];
}

ASTNodeType ASTNode::getType() {
  return type;
}

string ASTNode::getValue() {
  return value;
}

bool ASTNode::isEquals(IASTNode* other) {
  if (type == ASTNODE_VARIABLE || type == ASTNODE_CONSTANT) {
    return type == other->getType() && value == other->getValue();
  }
  return type == other->getType();
}
