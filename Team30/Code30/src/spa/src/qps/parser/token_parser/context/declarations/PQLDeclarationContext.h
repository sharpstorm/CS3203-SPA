#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

class PQLDeclarationContext: public AbstractPQLContext {
  PQL_VAR_TYPE variableType;

 public:
  void parse(TokenParseState* parserState);
  explicit PQLDeclarationContext(PQL_VAR_TYPE variableType);
};
