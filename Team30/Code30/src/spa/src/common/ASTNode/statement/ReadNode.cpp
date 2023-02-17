#include <string>
#include <vector>
#include <memory>
#include "ReadNode.h"

using std::vector, std::shared_ptr, std::to_string;

ReadNode::ReadNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
  type = ASTNODE_READ;
}

void ReadNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void ReadNode::addChild(shared_ptr<ASTNode> node) {
  children.push_back(node);
}

string ReadNode::toString() {
  return ":read";
}
