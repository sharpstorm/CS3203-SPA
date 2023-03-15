#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "qps/clauses/Clause.h"
#include "AttributedSynonym.h"
#include "qps/common/constraint/Constraint.h"
#include "qps/common/constraint/OverrideTransformer.h"

using std::string, std::unordered_map, std::vector, std::unique_ptr;

class PQLQuery {
 private:
  VariableTablePtr variables;
  AttributedSynonymList resultVariables;
  vector<ClauseSPtr> clauses;
  vector<ConstraintSPtr> constraints;

 public:
  PQLQuery(VariableTablePtr vars,
           AttributedSynonymList resVars,
           vector<ClauseSPtr> c,
           vector<ConstraintSPtr> con);
  int getVariableCount();
  VariableTable* getVarTable();
  AttributedSynonymList * getResultVariables();
  PQLQuerySynonym* getVariable(PQLSynonymName name);
  SynonymList getDeclaredSynonyms();
  vector<IEvaluatableSPtr> getEvaluatables();
  vector<ConstraintSPtr> getConstraints();
  int getClauseCount();
};

typedef unique_ptr<PQLQuery> PQLQueryPtr;
