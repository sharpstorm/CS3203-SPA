#pragma once

#include <memory>
#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"
#include "sp/common/ASTNode/math/ConditionalExpressionASTNode.h"

using std::shared_ptr;

class ConditionalExpressionContext: public RecursiveParseContext {
 public:
  explicit ConditionalExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<ConditionalExpressionASTNode>
      generateConditionalNode(SourceParseState* state,
                              shared_ptr<ASTNode> leftNode);
};