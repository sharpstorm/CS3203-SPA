#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ASTNode/math/PlusASTNode.h"
#include "common/ASTNode/math/MinusASTNode.h"

using std::shared_ptr;

class ExpressionContext: public RecursiveParseContext {
 public:
  explicit ExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<PlusASTNode>
  generatePlus(shared_ptr<ASTNode> leftNode);

  shared_ptr<MinusASTNode>
  generateMinus(shared_ptr<ASTNode> leftNode);
};
