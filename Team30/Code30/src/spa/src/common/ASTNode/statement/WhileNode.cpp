#include <string>
#include <vector>
#include <memory>
#include "WhileNode.h"

using std::vector, std::shared_ptr, std::string;

WhileNode::WhileNode() {
  // index [0] = conditional expression
  // index [1] = statement list
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
  type = ASTNODE_WHILE;
}

void WhileNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void WhileNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string WhileNode::toString() {
  string ss = ":while";
  ss += children[0] == nullptr ? "none" : children[0]->toString() + "\n";
  ss += "StmtLst\n";
  ss += children[1] == nullptr ? "none" : children[1]->toString() + "\n";
  return ss;
}

