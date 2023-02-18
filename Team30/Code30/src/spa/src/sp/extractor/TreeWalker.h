#pragma once

#include <vector>

#include "AbstractExtractor.h"
#include "common/AST.h"
#include "common/UtilityTypes.h"
#include "common/ASTNode/ASTNode.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast, VectorPtr<Extractor*> extractors);

 private:
  void DFS(ASTNodePtr ast, VectorPtr<Extractor*> extractors);
};
