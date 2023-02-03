#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(unordered_map<string, PQLQueryVariable> vars,
                   PQLQueryVariable resVar,
                   vector<shared_ptr<Clause>> c) {
  variables = vars;
  resultVariable = resVar;
  clauses = c;
}

int PQLQuery::getVariableCount() {
  return variables.size();
}

PQLQueryVariable* PQLQuery::getVariable(PQL_VAR_NAME name) {
  auto item = variables.find(name);
  if (item == variables.end()) {
    return nullptr;
  }

  return &item->second;
}

vector<shared_ptr<IEvaluatable>> PQLQuery::getEvaluatables() {
  vector<shared_ptr<IEvaluatable>> evals = vector<shared_ptr<IEvaluatable>>();
  for (shared_ptr<Clause> ie : clauses) {
      evals.push_back(ie);
  }

  return evals;
}
