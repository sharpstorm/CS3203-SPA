#include "TreeWalker.h"

#include <iostream>
#include "common/ASTNode/StatementListNode.h"

using std::cout, std::endl;

void TreeWalker::walkAST(AST ast,
                         vector<shared_ptr<Extractor>> extractors) {
  DFS(ast.getRoot(), extractors);
}

void TreeWalker::DFS(shared_ptr<ASTNode> node,
                     vector<shared_ptr<Extractor>> extractors) {
  for (int i = 0; i < extractors.size(); i++) {
    node->accept(extractors.at(i));
  }
  if (node->getChildren().size() > 0) {
    for (shared_ptr<ASTNode> child : node->getChildren()) {
      if (child != nullptr) {
        DFS(child, extractors);
      }
    }
  }
  for (int i = 0; i < extractors.size(); i++) {
    node->leave(extractors.at(i));
  }
}
