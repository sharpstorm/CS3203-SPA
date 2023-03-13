#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTable vars,
                   AttributedSynonymList resVar,
                   vector<ClauseSPtr> c,
                   vector<ConstraintSPtr> con):
                   variables(vars), resultVariables(resVar), clauses(c),
                   constraints(con) { }

int PQLQuery::getVariableCount() {
  return variables.size();
}

AttributedSynonymList* PQLQuery::getResultVariables() {
  return &resultVariables;
}

PQLQuerySynonym* PQLQuery::getVariable(PQLSynonymName name) {
  auto item = variables.find(name);
  if (item == variables.end()) {
    return nullptr;
  }

  return &item->second;
}

vector<IEvaluatableSPtr> PQLQuery::getEvaluatables() {
  vector<IEvaluatableSPtr> evals;
  for (ClauseSPtr ie : clauses) {
      evals.push_back(ie);
  }

  return evals;
}

int PQLQuery::getClauseCount() {
  return clauses.size();
}

vector<ConstraintSPtr> PQLQuery::getConstraints() {
  return constraints;
}
