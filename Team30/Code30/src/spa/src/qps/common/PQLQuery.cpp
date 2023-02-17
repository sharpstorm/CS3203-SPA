#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(unordered_map<string, PQLQuerySynonym> vars,
                   PQLQuerySynonym resVar,
                   vector<shared_ptr<Clause>> c):
                   variables(vars), resultVariable(resVar), clauses(c) {
}

int PQLQuery::getVariableCount() {
  return variables.size();
}

PQLQuerySynonym PQLQuery::getResultVariable() {
  return resultVariable;
}

PQLQuerySynonym* PQLQuery::getVariable(PQLSynonymName name) {
  auto item = variables.find(name);
  if (item == variables.end()) {
    return nullptr;
  }

  return &item->second;
}

PQLSynonymName PQLQuery::getResultName() {
  return resultVariable.getName();
}

vector<shared_ptr<IEvaluatable>> PQLQuery::getEvaluatables() {
  vector<shared_ptr<IEvaluatable>> evals = vector<shared_ptr<IEvaluatable>>();
  for (shared_ptr<Clause> ie : clauses) {
      evals.push_back(ie);
  }

  return evals;
}
