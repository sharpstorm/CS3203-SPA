#include "VariableASTNode.h"
#include "sp/extractor/Extractor.h"

VariableASTNode::VariableASTNode(string v): ASTNode(ASTNODE_VARIABLE, v) {}

void VariableASTNode::accept(Extractor* e) {
  e->visit(this);
}

void VariableASTNode::leave(Extractor* e) {
}

string VariableASTNode::toString() {
  return value;
}

string VariableASTNode::getValue() {
  return value;
}
