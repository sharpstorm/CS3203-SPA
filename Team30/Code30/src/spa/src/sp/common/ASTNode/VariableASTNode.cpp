#include "VariableASTNode.h"

VariableASTNode::VariableASTNode(string v) {
    value = v;
}

string VariableASTNode::toString() {
  return "Var(" + value + ")";
}
