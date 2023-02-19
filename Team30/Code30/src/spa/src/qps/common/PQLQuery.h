#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "qps/clauses/Clause.h"

using std::string, std::unordered_map, std::vector, std::shared_ptr;

class PQLQuery {
 private:
  VariableTable variables;
  PQLQuerySynonym resultVariable;
  vector<ClauseSPtr> clauses;
 public:
  PQLQuery(unordered_map<string, PQLQuerySynonym> vars,
           PQLQuerySynonym resVar,
           vector<ClauseSPtr> c);
  int getVariableCount();
  PQLQuerySynonym getResultVariable();
  PQLQuerySynonym* getVariable(PQLSynonymName name);
  PQLSynonymName getResultName();
  vector<IEvaluatableSPtr> getEvaluatables();
};
