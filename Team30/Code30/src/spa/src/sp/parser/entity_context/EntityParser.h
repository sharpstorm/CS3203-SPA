#pragma once

#include "IEntityParser.h"

class EntityParser : public IEntityParser {
 public:
  ASTNodePtr parseVariable(SourceParseState *state) const override;
  ASTNodePtr parseConstant(SourceParseState *state) const override;

 private:
  template<class T>
  ASTNodePtr makeNode(const SourceToken* token) const;
};
