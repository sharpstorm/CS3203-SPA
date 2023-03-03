#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "qps/clauses/Clause.h"

using std::string, std::unordered_map, std::vector;

class PQLQuery {
 private:
  VariableTable variables;
  PQLQuerySynonymList resultVariables;
  vector<ClauseSPtr> clauses;

 public:
  PQLQuery(VariableTable vars,
           PQLQuerySynonymList resVars,
           vector<ClauseSPtr> c);
  int getVariableCount();
  PQLQuerySynonymList* getResultVariables();
  PQLQuerySynonym* getVariable(PQLSynonymName name);
  vector<IEvaluatableSPtr> getEvaluatables();
  int getClauseCount();
};
