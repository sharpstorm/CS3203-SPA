#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ASTNode/ASTNode.h"

using std::shared_ptr;

class WhileContext : public RecursiveParseContext {
 public:
  explicit WhileContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};