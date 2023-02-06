#pragma once

#include <memory>
#include "SourceParseContext.h"
#include "../../common/ASTNode/ASTNode.h"

using std::shared_ptr;

class VariableParseContext : public SourceParseContext {
 public:
  VariableParseContext();
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
