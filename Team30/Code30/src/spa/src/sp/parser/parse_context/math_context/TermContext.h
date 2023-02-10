#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/parser/IGrammarContextProvider.h"
#include "common/ASTNode/math/TimesASTNode.h"

using std::shared_ptr;

class TermContext: public RecursiveParseContext {
 public:
  explicit TermContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
 private:
  shared_ptr<AbstractMathASTNode>
  generateTimes(shared_ptr<ASTNode> leftNode);
  shared_ptr<AbstractMathASTNode>
  generateDiv(shared_ptr<ASTNode> leftNode);
  shared_ptr<AbstractMathASTNode>
  generateMod(shared_ptr<ASTNode> leftNode);
};
