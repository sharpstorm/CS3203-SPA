#pragma once

#include "sp/parser/SourceParseState.h"
#include "sp/ast/ASTNode.h"

class SourceParseContext {
 public:
  virtual ~SourceParseContext() = default;
  virtual ASTNodePtr generateSubtree(SourceParseState *state) const = 0;
};
