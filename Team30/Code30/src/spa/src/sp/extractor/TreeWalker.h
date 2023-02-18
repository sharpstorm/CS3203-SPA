#pragma once

#include <vector>

#include "AbstractExtractor.h"
#include "common/AST.h"
#include "common/UtilityTypes.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast, VectorPtr<Extractor*> extractors);

 private:
  void DFS(shared_ptr<ASTNode> ast, VectorPtr<Extractor*> extractors);
};
