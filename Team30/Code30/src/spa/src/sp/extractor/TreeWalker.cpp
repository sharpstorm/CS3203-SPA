#include "TreeWalker.h"

#include "sp/ast/StatementListNode.h"

void TreeWalker::walkAST(const AST* ast,
                         const ExtractorRefList extractors) const {
  DFS(ast->getMutableRoot(), extractors);
}

void TreeWalker::DFS(const ASTNode* node,
                     const ExtractorRefList extractors) const {
  for (const auto &extractor : *extractors) {
    node->accept(extractor);
  }

  for (ASTNode* child : node->getChildren()) {
    if (child != nullptr) {
      DFS(child, extractors);
    }
  }

  for (const auto &extractor : *extractors) {
    node->leave(extractor);
  }
}
