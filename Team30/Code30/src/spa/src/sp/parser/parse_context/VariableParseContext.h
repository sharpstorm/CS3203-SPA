#pragma once

#include "SourceParseContext.h"
#include "common/ast/ASTNode.h"

class VariableParseContext : public SourceParseContext {
 public:
  VariableParseContext();
  ASTNodePtr generateSubtree(SourceParseState* state);
};
