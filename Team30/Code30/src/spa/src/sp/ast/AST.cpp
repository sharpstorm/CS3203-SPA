#include "AST.h"

#include <utility>

AST::AST(ASTNodePtr node) {
  root = std::move(node);
}

const ASTNode *AST::getMutableRoot() const {
  return root.get();
}

IASTNode *AST::getRoot() const {
  return root.get();
}
