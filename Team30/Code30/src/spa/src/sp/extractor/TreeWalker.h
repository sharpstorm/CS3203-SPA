#pragma once

#include "sp/common/AST.h"
class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast); // TODO implement with extractor
 private:
  void DFS(shared_ptr<ASTNode> ast);
};