#pragma once

#include <memory>

#include "sp/ast/ASTNode.h"
#include "sp/parser/SourceParseState.h"

using std::unique_ptr;

class IEntityParser {
 public:
  virtual ~IEntityParser() = default;
  virtual ASTNodePtr parseVariable(SourceParseState *state) const = 0;
  virtual ASTNodePtr parseConstant(SourceParseState *state) const = 0;
};

typedef unique_ptr<IEntityParser> IEntityParserPtr;
