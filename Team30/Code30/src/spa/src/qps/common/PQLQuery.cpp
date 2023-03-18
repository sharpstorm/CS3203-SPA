#include <utility>

#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTablePtr vars,
                   AttributedSynonymList resVar,
                   vector<ClauseSPtr> c,
                   vector<ConstraintSPtr> con):
                   variables(std::move(vars)), resultVariables(resVar),
                   clauses(c), constraints(con) { }

int PQLQuery::getVariableCount() {
  return variables->size();
}

VariableTable* PQLQuery::getVarTable() {
  return variables.get();
}

AttributedSynonymList* PQLQuery::getResultVariables() {
  return &resultVariables;
}

PQLQuerySynonym* PQLQuery::getVariable(PQLSynonymName name) {
  return variables->find(name);
}

SynonymList PQLQuery::getDeclaredSynonyms() {
  vector<string> result;
  for (auto i : variables->getReferredSynonyms()) {
    result.push_back(i->getName());
  }
  return result;
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
