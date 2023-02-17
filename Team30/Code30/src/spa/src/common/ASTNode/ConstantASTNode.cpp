#include <string>
#include "ConstantASTNode.h"

using std::string;

ConstantASTNode::ConstantASTNode(string v) {
  value = v;
}

string ConstantASTNode::toString() {
  return "Const:" + value;
}

void ConstantASTNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string ConstantASTNode::getValue() {
  return value;
}

ASTNodeType ConstantASTNode::getType() {
  return ASTNODE_CONSTANT;
}
