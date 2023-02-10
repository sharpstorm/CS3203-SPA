#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"

using std::shared_ptr;

class RelationalExpressionContext: public RecursiveParseContext {
 public:
  explicit RelationalExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<RelationalExpressionASTNode>
  generateRelationalNode(SourceParseState* state, shared_ptr<ASTNode> leftNode);
};
