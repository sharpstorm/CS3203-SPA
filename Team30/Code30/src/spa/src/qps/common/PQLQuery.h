#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQueryVariable.h"
#include "../clauses/Clause.h"
#include "IEvaluatable.h"

using std::string, std::unordered_map, std::vector, std::shared_ptr;

class PQLQuery {
    VariableTable variables;
    PQLQueryVariable resultVariable;
    vector<shared_ptr<Clause>> clauses;
 public:
    PQLQuery(unordered_map<string, PQLQueryVariable> vars,
             PQLQueryVariable resVar,
             vector<shared_ptr<Clause>> c);
    int getVariableCount();
    PQLQueryVariable* getVariable(PQL_VAR_NAME name);
    vector<shared_ptr<IEvaluatable>> getEvaluatables();
};
