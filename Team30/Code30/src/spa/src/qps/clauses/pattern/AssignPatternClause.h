#pragma once

#include <string>
#include <memory>
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"

using std::string, std::shared_ptr;

class AssignPatternClause: public PatternClause {
 private:
  PQLQuerySynonym assignSynonym;
  ClauseArgumentPtr leftArgument;
  ExpressionArgumentPtr rightArgument;

 public:
  AssignPatternClause(PQLQuerySynonym assignSynonym,
                      ClauseArgumentPtr leftArg,
                      ExpressionArgumentPtr rightArg);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  SynonymList getUsedSynonyms();
  bool matchPartial(shared_ptr<IASTNode> rootNode);
  bool matchExact(shared_ptr<IASTNode> rootNode);
};
