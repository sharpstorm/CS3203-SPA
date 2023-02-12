#pragma once

#include <string>
#include <memory>
#include "PatternClause.h"
#include "ClauseArgument.h"

using std::string;

class AssignPatternClause: public PatternClause {
 private:
  PQLQuerySynonym assignSynonym;
  ClauseArgument leftArgument;
  string patternPhrase;
  bool allowPartial;

 public:
  AssignPatternClause(PQLQuerySynonym assignSynonym,
                      ClauseArgument leftSynonym,
                      string patternPhrase,
                      bool allowPartial);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
