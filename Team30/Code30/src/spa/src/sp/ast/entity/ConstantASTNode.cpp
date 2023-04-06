#include "ConstantASTNode.h"

ConstantASTNode::ConstantASTNode(const ASTNodeValue &v) :
    ASTNode(ASTNODE_CONSTANT, v) {}

void ConstantASTNode::accept(IExtractor *e) const {
  e->visitConstant(this);
}
