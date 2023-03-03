#pragma once

#include "sp/common/SourceToken.h"
#include "sp/parser/SourceParseState.h"
#include "common/ast/ASTNode.h"
#include "sp/errors/SPError.h"

class SourceParseContext {
 public:
  virtual ~SourceParseContext() = default;
  virtual ASTNodePtr generateSubtree(SourceParseState* state) = 0;
};
