#pragma once

#include <string>
#include "PQLPatternClauseContext.h"

using std::string;

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
