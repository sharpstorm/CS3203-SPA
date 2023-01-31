#include "PQLQuery.h"

using std::pair;

PQLQuery::PQLQuery(vector<QueryVariable> vars, QueryVariable resVar, vector<Clause> c) {
    populateVariables(vars);
    resultVariable = resVar;
    clauses = c;
}

void PQLQuery::populateVariables(vector<QueryVariable> vars) {
    for (QueryVariable qv : vars) {
        variables.insert({qv.name, qv});
    }
}

PQL_VAR_TYPE PQLQuery::getSymbol(PQL_VAR_NAME name) {
    QueryVariable qVar = variables[name];
    return qVar.type;
}

vector<IEvaluatable>* PQLQuery::getEvaluatables() {
    vector<IEvaluatable>* evals = new vector<IEvaluatable>();
    for (IEvaluatable ie : clauses) {
        evals->push_back(ie);
    }

    return evals;
}