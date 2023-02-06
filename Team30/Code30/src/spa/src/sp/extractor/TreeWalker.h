#pragma once

#include <memory>
#include <vector>

#include "AbstractExtractor.h"
#include "sp/common/AST.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast, vector<shared_ptr<Extractor>> extractors);

 private:
  void DFS(shared_ptr<ASTNode> ast, vector<shared_ptr<Extractor>> extractors);
};
