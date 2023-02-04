#include "VariableASTNode.h"

VariableASTNode::VariableASTNode(string v) {
    value = v;
}

void VariableASTNode::accept() {
}

string VariableASTNode::toString() {
  return "Var(" + value + ")";
}
