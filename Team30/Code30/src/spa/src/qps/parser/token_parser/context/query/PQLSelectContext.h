#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include <string>

using std::string;

class PQLSelectContext: public AbstractPQLContext {
 public:
  void parse(QueryTokenParseState* parserState);

 private:
  void parseTuple(QueryTokenParseState* parserState);
  void addResultSynonym(QueryTokenParseState* parserState, string synName);
};
