#pragma once

#include <string>
#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

using std::string;

class PQLDeclarationContext: public AbstractPQLContext {
  PQLSynonymType synonymType;

 public:
  void parse(QueryTokenParseState* parserState);

 protected:
  explicit PQLDeclarationContext(PQLSynonymType synonymType);

 private:
  void addVariableToState(
      string name,
      QueryTokenParseState* parserState);
};
