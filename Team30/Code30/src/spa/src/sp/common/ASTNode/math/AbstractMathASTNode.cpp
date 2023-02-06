#include <vector>
#include <memory>
#include "AbstractMathASTNode.h"

using std::vector, std::shared_ptr;

void AbstractMathASTNode::accept(shared_ptr<Extractor> e) {}

AbstractMathASTNode::AbstractMathASTNode(string repr) : repr(repr) {
  children = vector<shared_ptr<ASTNode>>{ nullptr, nullptr };
}

string AbstractMathASTNode::toString() {
  return repr + ":  left:" + getLeftChild()->toString()
              + ": right: " + getRightChild()->toString();
}
