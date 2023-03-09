#include "TreeWalker.h"

#include "sp/ast/StatementListNode.h"

void TreeWalker::walkAST(AST* ast,
                         VectorPtr<IExtractor*> extractors) {
  DFS(ast->getMutableRoot(), extractors);
}

void TreeWalker::DFS(ASTNode* node, VectorPtr<IExtractor*> extractors) {
  for (int i = 0; i < extractors->size(); i++) {
    node->accept(extractors->at(i));
  }

  for (ASTNode* child : node->getChildren()) {
    if (child != nullptr) {
      DFS(child, extractors);
    }
  }

  for (int i = 0; i < extractors->size(); i++) {
    node->leave(extractors->at(i));
  }
}
