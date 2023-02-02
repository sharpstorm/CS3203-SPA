#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(vector<QueryVariable> vars, QueryVariable resVar,
                   vector<Clause*> c) {
    populateVariables(vars);
    resultVariable = resVar;
    clauses = c;
}

void PQLQuery::populateVariables(vector<QueryVariable> vars) {
    for (QueryVariable qv : vars) {
        variables.insert({qv.name, qv});
    }
}

int PQLQuery::getVariableCount() {
  return variables.size();
}

QueryVariable* PQLQuery::getVariable(PQL_VAR_NAME name) {
  auto item = variables.find(name);
  if (item == variables.end()) {
    return nullptr;
  }

  return &item->second;
}

vector<IEvaluatable*> PQLQuery::getEvaluatables() {
    vector<IEvaluatable*> evals = vector<IEvaluatable*>();
    for (IEvaluatable* ie : clauses) {
        evals.push_back(ie);
    }

    return evals;
}
