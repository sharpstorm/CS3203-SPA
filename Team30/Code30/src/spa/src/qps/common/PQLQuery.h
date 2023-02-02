#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLVariable.h"
#include "../clauses/Clause.h"
#include "IEvaluatable.h"

using std::string, std::unordered_map, std::vector;

class PQLQuery {
    VariableTable variables;
    QueryVariable resultVariable;
    vector<Clause*> clauses;
    void populateVariables(vector<QueryVariable> vars);
 public:
    PQLQuery(vector<QueryVariable> vars, QueryVariable resVar,
             vector<Clause*> c);
    int getVariableCount();
    QueryVariable* getVariable(PQL_VAR_NAME name);
    vector<IEvaluatable*> getEvaluatables();
};
