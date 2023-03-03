#pragma once

#include "IEntityParser.h"

class EntityParser: public IEntityParser {
 public:
  ASTNodePtr parseVariable(SourceParseState* state);
  ASTNodePtr parseConstant(SourceParseState* state);
};
