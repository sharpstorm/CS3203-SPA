#pragma once

#include <string>
#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

using std::string;

class PQLSelectContext: public AbstractPQLContext {
 public:
  void parse(QueryTokenParseState* parserState);

 private:
  void parseTuple(QueryTokenParseState* parserState);
  void addResultSynonym(QueryTokenParseState* parserState, string synName);
};
