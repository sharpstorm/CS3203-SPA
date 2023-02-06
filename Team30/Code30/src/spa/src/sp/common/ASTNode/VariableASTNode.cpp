#include "VariableASTNode.h"
#include "sp/extractor/Extractor.h"

VariableASTNode::VariableASTNode(string v) {
    value = v;
}

void VariableASTNode::accept(shared_ptr<Extractor> e) {}

string VariableASTNode::toString() {
  return "Var(" + value + ")";
}