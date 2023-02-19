#pragma once

#include "SourceParseContext.h"
#include "common/ASTNode/ASTNode.h"

class VariableParseContext : public SourceParseContext {
 public:
  VariableParseContext();
  ASTNodePtr generateSubtree(SourceParseState* state);
};
