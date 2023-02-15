#pragma once

#include "PQLPatternClauseContext.h"

class PQLAssignPatternClauseContext : public PQLPatternClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);

 private:
  struct PatternData {
    bool isPartial;
    string pattern;
  };

  PatternData extractPatternData(QueryTokenParseState* parserState);
};
