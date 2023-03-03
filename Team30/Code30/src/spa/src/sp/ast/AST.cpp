#include "AST.h"

AST::AST(ASTNodePtr node) {
  root = node;
}

ASTNodePtr AST::getMutableRoot() {
  return root;
}

IASTNodePtr AST::getRoot() {
  return root;
}
