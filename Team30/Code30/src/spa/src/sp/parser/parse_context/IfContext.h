#pragma once

#include <memory>
#include "RecursiveParseContext.h"
#include "../../common/ASTNode/ASTNode.h"

using std::shared_ptr;

class IfContext : public RecursiveParseContext {
 public:
  explicit IfContext(IGrammarContextProvider* provider):
      RecursiveParseContext(provider) {}
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
