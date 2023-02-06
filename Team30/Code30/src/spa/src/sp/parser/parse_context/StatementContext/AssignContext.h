#pragma once

#include <memory>
#include "sp/parser/parse_context/RecursiveParseContext.h"
#include "sp/common/ASTNode/ASTNode.h"

using std::shared_ptr;

class AssignContext : public RecursiveParseContext {
 public:
  explicit AssignContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
