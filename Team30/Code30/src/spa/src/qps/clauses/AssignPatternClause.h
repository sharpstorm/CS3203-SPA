#pragma once

#include <string>
#include <memory>
#include "PatternClause.h"
#include "arguments/ClauseArgument.h"

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
  bool usesSynonym(string varName);
  bool findExpression(shared_ptr<IASTNode> rootNode);
};
