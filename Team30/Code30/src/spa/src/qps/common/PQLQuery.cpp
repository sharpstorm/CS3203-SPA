#include <utility>

#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTablePtr vars,
                   const AttributedSynonymList &resVar,
                   vector<ClausePtr> c,
                   vector<ConstraintPtr> con):
                   variables(std::move(vars)), resultVariables(resVar),
                   clauses(std::move(c)), constraints(std::move(con)) { }

VariableTable* PQLQuery::getVarTable() {
  return variables.get();
}

const AttributedSynonymList* PQLQuery::getResultVariables() const {
  return &resultVariables;
}

PQLQuerySynonymProxy* PQLQuery::getVariable(PQLSynonymName name) {
  return variables->find(name);
}

const PQLSynonymNameList PQLQuery::getDeclaredSynonyms() const {
  PQLSynonymNameList result;
  for (auto i : variables->getReferredSynonyms()) {
    result.push_back(i);
  }

  return result;
}

const vector<IEvaluatable*> PQLQuery::getEvaluatables() const {
  vector<IEvaluatable*> evals;
  for (const ClausePtr &ie : clauses) {
      evals.push_back(ie.get());
  }

  return evals;
}

int PQLQuery::getClauseCount() const {
  return clauses.size();
}

const vector<Constraint*> PQLQuery::getConstraints() const {
  vector<Constraint*> ret;
  for (const ConstraintPtr &c : constraints) {
    ret.push_back(c.get());
  }

  return ret;
}

bool PQLQuery::isBooleanResult() const {
  return resultVariables.empty();
}
