#pragma once

#include <string>
#include "PQLPatternClauseContext.h"
#include "qps/common/PQLQuerySynonym.h"

using std::string;

class PQLAssignPatternClauseContext : public PQLPatternClauseContext {
 public:
  explicit PQLAssignPatternClauseContext(PQLQuerySynonym* synonym):
      PQLPatternClauseContext(synonym) {}
  void parse(QueryTokenParseState* parserState);

 private:
  struct PatternData {
    bool isPartial;
    string pattern;
  };

  PatternData extractPatternData(QueryTokenParseState* parserState);
};
