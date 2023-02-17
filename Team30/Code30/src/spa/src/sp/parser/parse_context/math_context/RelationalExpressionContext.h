#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"

using std::shared_ptr;

class RelationalExpressionContext: public RecursiveParseContext {
 public:
  explicit RelationalExpressionContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

 private:
  shared_ptr<BinaryASTNode>
  generateRelationalNode(SourceTokenType type, shared_ptr<ASTNode> leftNode);
};
