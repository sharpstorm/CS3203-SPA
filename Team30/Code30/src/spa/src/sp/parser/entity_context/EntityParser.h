#pragma once

#include "IEntityParser.h"

class EntityParser : public IEntityParser {
 public:
  ASTNodePtr parseVariable(SourceParseState *state) const override;
  ASTNodePtr parseConstant(SourceParseState *state) const override;
};
