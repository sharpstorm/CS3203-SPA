#pragma once

#include <memory>
#include "SourceParseContext.h"
#include "../../common/ASTNode/ASTNode.h"

using std::shared_ptr;

class ConstantParseContext : public SourceParseContext {
 public:
  ConstantParseContext();
  bool validate(SourceParseState* state);
  shared_ptr<ASTNode> generateSubtree(SourceParseState* state);
};
