#include "TreeWalker.h"

#include <iostream>
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"


using std::cout, std::endl;

void TreeWalker::walkAST(AST ast) {
  shared_ptr<Extractor> f = shared_ptr<Extractor>(new FollowsExtractor());
  DFS(ast.getRoot(), f);
}

void TreeWalker::DFS(shared_ptr<ASTNode> node, shared_ptr<Extractor> f) {
//  FollowsExtractor f;
  node->accept(f);
//  cout << node->toString() << endl;
  if (node->getChildren().size() > 0) {
    // Iterate through node's children
    for (shared_ptr<ASTNode> child : node->getChildren()) {
        DFS(child, f);
    }
  }
}
