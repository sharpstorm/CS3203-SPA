#include "VariableASTNode.h"
#include "sp/extractor/Extractor.h"

VariableASTNode::VariableASTNode(string v): ASTNode(ASTNODE_VARIABLE, v) {}

void VariableASTNode::accept(Extractor* e) {
  e->visitVariable(this);
}

string VariableASTNode::toString() {
  return value;
}
