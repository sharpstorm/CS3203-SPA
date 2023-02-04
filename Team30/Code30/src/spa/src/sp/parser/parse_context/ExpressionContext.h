#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"
#include "sp/common/ASTNode/math/PlusASTNode.h"
#include "sp/common/ASTNode/math/MinusASTNode.h"

class ExpressionContext: public RecursiveParseContext {
 public:
  ExpressionContext(IGrammarContextProvider* provider):
  RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<PlusASTNode> generatePlus(SourceParseState* state, shared_ptr<ASTNode> leftNode);
  shared_ptr<MinusASTNode> generateMinus(SourceParseState* state, shared_ptr<ASTNode> leftNode);
};
