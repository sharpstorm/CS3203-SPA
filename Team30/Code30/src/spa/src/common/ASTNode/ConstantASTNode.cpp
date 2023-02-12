#include <string>
#include "ConstantASTNode.h"

using std::string;

ConstantASTNode::ConstantASTNode(string v) {
  value = v;
}

string ConstantASTNode::toString() {
  return "const(" + value + ")";
}

void ConstantASTNode::accept(shared_ptr<Extractor> e) {
}

string ConstantASTNode::getValue() {
  return value;
}
