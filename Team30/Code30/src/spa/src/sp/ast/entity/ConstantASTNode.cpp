#include <string>
#include "ConstantASTNode.h"

using std::string;

ConstantASTNode::ConstantASTNode(string v) : ASTNode(ASTNODE_CONSTANT, v) {
}

string ConstantASTNode::toString() {
  return value;
}

void ConstantASTNode::accept(IExtractor* e) {
  e->visitConstant(this);
}
