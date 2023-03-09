#include <memory>
#include "ProgramNode.h"
#include "sp/extractor/Extractor.h"

using std::shared_ptr;

ProgramNode::ProgramNode(): ASTNode(ASTNODE_PROGRAM) {
}

string ProgramNode::toString() {
  string ss;
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  ss += "}";
  return ss;
}
