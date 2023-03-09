#pragma once

#include "IEntityParser.h"

class EntityParser: public IEntityParser {
 public:
  ASTNodePtr parseVariable(SourceParseState* state) override;
  ASTNodePtr parseConstant(SourceParseState* state) override;
};
