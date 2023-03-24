#include <utility>

#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTablePtr vars,
                   AttributedSynonymList resVar,
                   vector<ClausePtr> c,
                   vector<ConstraintPtr> con):
                   variables(std::move(vars)), resultVariables(resVar),
                   clauses(std::move(c)), constraints(std::move(con)) { }

VariableTable* PQLQuery::getVarTable() {
  return variables.get();
}

AttributedSynonymList* PQLQuery::getResultVariables() {
  return &resultVariables;
}

PQLQuerySynonymProxy* PQLQuery::getVariable(PQLSynonymName name) {
  return variables->find(name);
}

SynonymList PQLQuery::getDeclaredSynonyms() {
  SynonymList result;
  for (auto i : variables->getReferredSynonyms()) {
    result.push_back(i);
  }

  return result;
}

vector<IEvaluatable*> PQLQuery::getEvaluatables() {
  vector<IEvaluatable*> evals;
  for (const ClausePtr &ie : clauses) {
      evals.push_back(ie.get());
  }

  return evals;
}

int PQLQuery::getClauseCount() {
  return clauses.size();
}

vector<Constraint*> PQLQuery::getConstraints() {
  vector<Constraint*> ret;
  for (const ConstraintPtr &c : constraints) {
    ret.push_back(c.get());
  }

  return ret;
}

bool PQLQuery::isBooleanResult() {
  return resultVariables.empty();
}
