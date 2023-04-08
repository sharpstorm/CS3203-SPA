#include <utility>

#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTablePtr vars,
                   const AttributedSynonymList &resVar,
                   vector<ClausePtr> c,
                   vector<ConstraintPtr> con):
                   variables(std::move(vars)), resultVariables(resVar),
                   clauses(std::move(c)), constraints(std::move(con)) { }

VariableTable* PQLQuery::getVarTable() const {
  return variables.get();
}

const AttributedSynonymList* PQLQuery::getResultVariables() const {
  return &resultVariables;
}

PQLQuerySynonymProxy* PQLQuery::getVariable(const PQLSynonymName &name) const {
  return variables->find(name);
}

const vector<IEvaluatable*> PQLQuery::getEvaluatables() const {
  vector<IEvaluatable*> evals;
  for (const ClausePtr &ie : clauses) {
      evals.push_back(ie.get());
  }

  return evals;
}

const vector<IConstraint*> PQLQuery::getConstraints() const {
  vector<IConstraint*> ret;
  for (const ConstraintPtr &c : constraints) {
    ret.push_back(c.get());
  }

  return ret;
}

int PQLQuery::getClauseCount() const {
  return clauses.size();
}

bool PQLQuery::isBooleanResult() const {
  return resultVariables.empty();
}
