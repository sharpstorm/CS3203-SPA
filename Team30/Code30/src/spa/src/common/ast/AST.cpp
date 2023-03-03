#include "AST.h"

AST::AST(ASTNodePtr node) {
  root = node;
}

ASTNodePtr AST::getRoot() {
  return root;
}
