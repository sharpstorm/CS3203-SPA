#include "TreeWalker.h"
#include "sp/common/ASTNode/statement/StatementASTNode.h"
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"
#include <iostream>

using std::cout, std::endl;

void TreeWalker::walkAST(AST ast) {
  DFS(ast.getRoot());
}

void TreeWalker::DFS(shared_ptr<ASTNode> node) {
  FollowsExtractor f;
//  node->accept(f);
  if (node->getChildren().size() > 0) {
    // Iterate through node's children
    for (shared_ptr<ASTNode> child : node->getChildren()) {
      // Dynamic cast to check if child is a statement
      if (std::dynamic_pointer_cast<StatementListNode>(child) != nullptr) {
        // TODO StatementList children size 0 check
        for (shared_ptr<ASTNode> baby : child->getChildren()) {
          DFS(baby);
        }
      }
    }
  }
}