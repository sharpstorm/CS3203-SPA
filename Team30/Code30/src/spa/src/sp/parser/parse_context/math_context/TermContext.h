#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ast/math/math_operand/TimesASTNode.h"

using std::shared_ptr;

class TermContext: public RecursiveParseContext {
 public:
  explicit TermContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
 private:
  shared_ptr<BinaryASTNode>
  generateOperand(SourceToken* curToken);
};
