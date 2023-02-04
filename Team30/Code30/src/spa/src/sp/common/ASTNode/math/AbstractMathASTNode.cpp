#include "AbstractMathASTNode.h"

using std::make_unique;

AbstractMathASTNode::AbstractMathASTNode(string repr) : repr(repr) {
  children = vector<shared_ptr<ASTNode>>{ nullptr, nullptr };
}

string AbstractMathASTNode::toString() {
  return repr + ":  left:" + getLeftChild()->toString()
              + ": right: " + getRightChild()->toString();
}

void AbstractMathASTNode::accept() {

}
