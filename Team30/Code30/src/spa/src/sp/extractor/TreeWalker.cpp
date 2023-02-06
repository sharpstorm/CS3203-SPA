#include "TreeWalker.h"

#include <iostream>
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"


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
    // Iterate through node's children
    for (shared_ptr<ASTNode> child : node->getChildren()) {
        DFS(child, extractors);
    }
  }
}
