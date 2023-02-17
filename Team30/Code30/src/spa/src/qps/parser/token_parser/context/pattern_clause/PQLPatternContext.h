#pragma once

#include "../AbstractPQLContext.h"
#include "qps/common/PQLQuerySynonym.h"

class PQLPatternContext: public AbstractPQLContext {
 public:
  void parse(QueryTokenParseState* parserState);

 private:
  void parsePatternClause(QueryTokenParseState* parserState);
  PQLQuerySynonym* parseSynonym(QueryTokenParseState* parserState);
  void dispatchPatternContext(PQLQuerySynonym* synonym,
                              QueryTokenParseState* parserState);
};
