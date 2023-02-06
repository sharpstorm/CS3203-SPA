#pragma once

#include <memory>
#include "sp/common/AST.h"
#include "AbstractExtractor.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast,
               vector<shared_ptr<Extractor>> extractors);
 private:
  void DFS(shared_ptr<ASTNode> ast,
           vector<shared_ptr<Extractor>> extractors);
};
