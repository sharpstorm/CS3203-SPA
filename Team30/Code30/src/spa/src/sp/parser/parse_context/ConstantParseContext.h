#pragma once

#include "SourceParseContext.h"
#include "common/ast/ASTNode.h"

class ConstantParseContext : public SourceParseContext {
 public:
  ConstantParseContext();
  ASTNodePtr generateSubtree(SourceParseState* state);
};
