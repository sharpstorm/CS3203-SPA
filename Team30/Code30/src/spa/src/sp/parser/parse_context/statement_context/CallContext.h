#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "common/ASTNode/ASTNode.h"

using std::shared_ptr;

class CallContext : public RecursiveParseContext {
 public:
  explicit CallContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
