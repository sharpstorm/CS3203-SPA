#include "ASTNode.h"

#include <utility>

ASTNode::ASTNode(ASTNodeType t) : type(t), value("") {
}

ASTNode::ASTNode(ASTNodeType t, string v) : type(t), value(v) {
}

vector<ASTNode*> ASTNode::getChildren() {
  vector<ASTNode*> result;
  for (int i = 0; i < children.size(); i++) {
    result.push_back(children.at(i).get());
  }

  return result;
}

void ASTNode::setChild(int index, ASTNodePtr node) {
  children[index] = std::move(node);
}

void ASTNode::addChild(ASTNodePtr node) {
  children.push_back(std::move(node));
}

IASTNode* ASTNode::getChild(int index) {
  if (index < 0 || index >= children.size()) {
    return nullptr;
  }

  return children[index].get();
}

int ASTNode::getChildCount() {
  return children.size();
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
