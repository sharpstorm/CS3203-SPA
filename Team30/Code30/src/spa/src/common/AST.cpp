#include "AST.h"
#include <memory>

using std::shared_ptr;

AST::AST(shared_ptr<ASTNode> node) {
  root = node;
}

shared_ptr<ASTNode> AST::getRoot() {
  return root;
}
