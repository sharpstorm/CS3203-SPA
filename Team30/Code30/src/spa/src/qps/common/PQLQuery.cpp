#include <utility>

#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(VariableTablePtr vars,
                   const AttributedSynonymList &resVar,
                   vector<ClausePtr> c,
                   vector<ConstraintPtr> con) :
    variables(std::move(vars)), resultVariables(resVar),
    clauses(std::move(c)), constraints(std::move(con)),
    hasAppliedConstraints(false) {}

const AttributedSynonymList *PQLQuery::getResultVariables() const {
  return &resultVariables;
}

PQLSynonymNameListPtr PQLQuery::getConstrainedVariables() const {
  auto ret = make_unique<PQLSynonymNameList>();
  for (const ConstraintPtr &c : constraints) {
    for (const PQLSynonymName &name : c->getAffectedSyns()) {
      ret->push_back(name);
    }
  }
  return std::move(ret);
}

PQLQuerySynonymProxy *PQLQuery::getVariable(const PQLSynonymName &name) const {
  return variables->find(name);
}

vector<IEvaluatable *> PQLQuery::getEvaluatables() const {
  vector<IEvaluatable *> evals;
  evals.reserve(clauses.size());
  for (const ClausePtr &ie : clauses) {
    evals.push_back(ie.get());
  }

  return evals;
}

int PQLQuery::getClauseCount() const {
  return clauses.size();
}

bool PQLQuery::isBooleanResult() const {
  return resultVariables.empty();
}

int PQLQuery::getDeclaredVariableCount() const {
  return variables->size();
}

bool PQLQuery::resolveConstraints(OverrideTable *table) {
  if (hasAppliedConstraints) {
    return true;
  }

  hasAppliedConstraints = true;
  SynonymProxyBuilder synProxyBuilder(variables->getProxyMap());

  for (const auto &con : constraints) {
    if (!con->applyConstraint(&synProxyBuilder, table)) {
      return false;
    }
  }

  synProxyBuilder.build();
  return synProxyBuilder.resolveOverrideMerging(table);
}
