#pragma once

#include <vector>

#include "AbstractExtractor.h"
#include "common/ast/AST.h"
#include "common/UtilityTypes.h"
#include "common/ast/ASTNode.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST ast, VectorPtr<Extractor*> extractors);

 private:
  void DFS(ASTNodePtr ast, VectorPtr<Extractor*> extractors);
};
