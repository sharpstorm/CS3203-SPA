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
  e->visit(*this);
}

string ConstantASTNode::getValue() {
  return value;
}
