#include "TreeWalker.h"

#include "sp/ast/StatementListNode.h"

void TreeWalker::walkAST(AST ast,
                         VectorPtr<Extractor*> extractors) {
  DFS(ast.getMutableRoot(), extractors);
}

void TreeWalker::DFS(ASTNodePtr node,
                     VectorPtr<Extractor*> extractors) {
  for (int i = 0; i < extractors->size(); i++) {
    node->accept(extractors->at(i));
  }
  if (node->getChildren().size() > 0) {
    for (ASTNodePtr child : node->getChildren()) {
      if (child != nullptr) {
        DFS(child, extractors);
      }
    }
  }
  for (int i = 0; i < extractors->size(); i++) {
    node->leave(extractors->at(i));
  }
}
