#pragma once

#include "AbstractExtractor.h"
#include "sp/ast/AST.h"
#include "common/UtilityTypes.h"
#include "sp/ast/ASTNode.h"

typedef VectorPtr<IExtractor*> ExtractorRefList;

class TreeWalker {
 public:
  TreeWalker() = default;
  void walkAST(const AST* ast, const ExtractorRefList extractors) const;

 private:
  void DFS(const ASTNode* ast, const ExtractorRefList extractors) const;
};
