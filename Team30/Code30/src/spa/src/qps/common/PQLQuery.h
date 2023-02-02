#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLQueryVariable.h"
#include "../clauses/Clause.h"
#include "IEvaluatable.h"

using std::string, std::unordered_map, std::vector;

class PQLQuery {
    VariableTable variables;
    PQLQueryVariable resultVariable;
    vector<Clause*> clauses;
 public:
    PQLQuery(unordered_map<string, PQLQueryVariable> vars,
             PQLQueryVariable resVar,
             vector<Clause*> c);
    int getVariableCount();
    PQLQueryVariable* getVariable(PQL_VAR_NAME name);
    vector<IEvaluatable*> getEvaluatables();
};
