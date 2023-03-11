#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "qps/clauses/Clause.h"
#include "AttributedSynonym.h"

using std::string, std::unordered_map, std::vector, std::unique_ptr;

class PQLQuery {
 private:
  VariableTable variables;
  AttributedSynonymList resultVariables;
  vector<ClauseSPtr> clauses;

 public:
  PQLQuery(VariableTable vars,
           AttributedSynonymList resVars,
           vector<ClauseSPtr> c);
  int getVariableCount();
  AttributedSynonymList * getResultVariables();
  PQLQuerySynonym* getVariable(PQLSynonymName name);
  vector<IEvaluatableSPtr> getEvaluatables();
  int getClauseCount();
};

typedef unique_ptr<PQLQuery> PQLQueryPtr;
