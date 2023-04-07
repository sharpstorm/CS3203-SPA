#include "ASTNode.h"

#include <utility>

ASTNode::ASTNode(ASTNodeType t) : type(t), value("") {
}

ASTNode::ASTNode(ASTNodeType t, const ASTNodeValue &v) : type(t), value(v) {
}

const ASTNodeRefList ASTNode::getChildren() const {
  ASTNodeRefList result;
  for (const auto &child : children) {
    result.push_back(child.get());
  }

  return result;
}

void ASTNode::setChild(int index, ASTNodePtr node) {
  children[index] = std::move(node);
}

void ASTNode::addChild(ASTNodePtr node) {
  children.push_back(std::move(node));
}

IASTNode *ASTNode::getChild(int index) const {
  if (index < 0 || index >= children.size()) {
    return nullptr;
  }

  return children[index].get();
}

int ASTNode::getChildCount() const {
  return children.size();
}

ASTNodeType ASTNode::getType() const {
  return type;
}

ASTNodeValue ASTNode::getValue() const {
  return value;
}

bool ASTNode::isEquals(IASTNode *other) const {
  if (type == ASTNODE_VARIABLE || type == ASTNODE_CONSTANT) {
    return type == other->getType() && value == other->getValue();
  }
  return type == other->getType();
}

ASTNode *ASTNode::getMutableChild(int index) const {
  return getChildren().at(index);
}
