#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ASTNode/BinaryASTNode.h"

using std::shared_ptr;

class ConditionalExpressionContext: public RecursiveParseContext {
 public:
  explicit ConditionalExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  template<class T>
  static shared_ptr<BinaryASTNode>
  generateConditionalNode(shared_ptr<ASTNode> leftNode);
  shared_ptr<ASTNode>
  processNotCondition(SourceParseState* state);
  shared_ptr<ASTNode>
  processBiCondition(SourceParseState* state);
  shared_ptr<ASTNode>
  processRelationalExpression(SourceParseState* state);
};
