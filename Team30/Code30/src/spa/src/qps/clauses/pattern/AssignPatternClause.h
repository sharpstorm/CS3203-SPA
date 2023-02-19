#pragma once

#include <string>
#include <memory>
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"

using std::string, std::shared_ptr;

class AssignPatternClause: public PatternClause {
 private:
  PQLQuerySynonym assignSynonym;
  ClauseArgumentPtr leftArgument;
  string patternPhrase;
  bool allowPartial;

 public:
  AssignPatternClause(PQLQuerySynonym assignSynonym,
                      ClauseArgumentPtr leftSynonym,
                      string patternPhrase,
                      bool allowPartial);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  SynonymList getUsedSynonyms();
  bool findExpression(shared_ptr<IASTNode> rootNode);
};