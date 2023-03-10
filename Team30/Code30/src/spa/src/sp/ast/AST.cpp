#include "AST.h"

#include <utility>

AST::AST(ASTNodePtr node) {
  root = std::move(node);
}

ASTNode* AST::getMutableRoot() {
  return root.get();
}

IASTNode* AST::getRoot() {
  return root.get();
}
