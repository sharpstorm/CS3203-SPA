#include "ProgramNode.h"
#include "sp/extractor/IExtractor.h"

ProgramNode::ProgramNode(): ASTNode(ASTNODE_PROGRAM) {
}

string ProgramNode::toString() {
  string ss;
  for (const ASTNodePtr &node : children) {
    ss += node->toString() + "\n";
  }
  ss += "}";
  return ss;
}
