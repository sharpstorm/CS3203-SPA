#include <string>
#include "ConstantASTNode.h"

using std::string;

ConstantASTNode::ConstantASTNode(string v) {
  value = v;
}

string ConstantASTNode::toString() {
  return value;
}

void ConstantASTNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void ConstantASTNode::leave(shared_ptr<Extractor> e) {
}

string ConstantASTNode::getValue() {
  return value;
}
