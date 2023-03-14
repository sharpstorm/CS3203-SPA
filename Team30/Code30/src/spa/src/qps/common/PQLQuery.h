#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "qps/clauses/Clause.h"
#include "qps/common/constraint/OverrideTransformer.h"

using std::string, std::unordered_map, std::vector, std::unique_ptr;

typedef unordered_map<string, OverrideTransformer> OverrideTable;

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

typedef unique_ptr<PQLQuery> PQLQueryPtr;
