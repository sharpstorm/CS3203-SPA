#pragma once

#include <vector>

#include "AbstractExtractor.h"
#include "sp/ast/AST.h"
#include "common/UtilityTypes.h"
#include "sp/ast/ASTNode.h"

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(AST* ast, VectorPtr<IExtractor*> extractors);

 private:
  void DFS(ASTNode* ast, VectorPtr<IExtractor*> extractors);
};
