#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

class PQLDeclarationContext: public AbstractPQLContext {
  PQLSynonymType variableType;

 public:
  void parse(QueryTokenParseState* parserState);

 protected:
  explicit PQLDeclarationContext(PQLSynonymType variableType);

 private:
  void addVariableToState(
      string name,
      QueryTokenParseState* parserState);
};
