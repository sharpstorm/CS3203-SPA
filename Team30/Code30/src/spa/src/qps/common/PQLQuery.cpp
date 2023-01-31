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

//void PQLQuery::addSymbol(PQL_VAR_NAME name, PQL_VAR_TYPE type) {
//    QueryVariable qVar = {type, name};
//    pair<string, QueryVariable> varEntry(name, qVar);
//    variables.insert(varEntry);
//}

PQL_VAR_TYPE PQLQuery::getSymbol(PQL_VAR_NAME name) {
    QueryVariable qVar = variables[name];
    return qVar.type;
}