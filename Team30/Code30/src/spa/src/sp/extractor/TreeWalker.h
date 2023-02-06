#pragma once

#include <memory>
#include "sp/common/AST.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast);  // TODO(Wei-Xinn) implement with extractor
 private:
  void DFS(shared_ptr<ASTNode> ast, shared_ptr<Extractor> f);
};
