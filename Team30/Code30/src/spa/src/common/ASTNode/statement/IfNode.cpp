#include <vector>
#include <memory>
#include <string>
#include "IfNode.h"

using std::vector, std::shared_ptr, std::string;

IfNode::IfNode() {
  // index [0] = conditional expression
  // index [1] = Then StmtLst
  // index [2] = Else StmtLst
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr, nullptr};
}

void IfNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void IfNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string IfNode::toString() {
  string ss = ":if";
  return ss;
}

ASTNodeType IfNode::getType() {
  return ASTNODE_IF;
}