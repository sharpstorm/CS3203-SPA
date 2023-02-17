#include "VariableASTNode.h"
#include "sp/extractor/Extractor.h"

VariableASTNode::VariableASTNode(string v) {
  value = v;
  type = ASTNODE_VARIABLE;
}

void VariableASTNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string VariableASTNode::toString() {
  return "Var:" + value;
}

string VariableASTNode::getValue() {
  return value;
}
