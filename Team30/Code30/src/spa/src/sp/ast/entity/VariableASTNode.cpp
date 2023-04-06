#include "VariableASTNode.h"

VariableASTNode::VariableASTNode(const ASTNodeValue &v) :
    ASTNode(ASTNODE_VARIABLE, v) {}

void VariableASTNode::accept(IExtractor *e) const {
  e->visitVariable(this);
}
