#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "PQLQueryVariable.h"
#include "qps/clauses/Clause.h"

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
    PQLQueryVariable getResultVariable();
    PQLQueryVariable* getVariable(PQL_SYN_NAME name);
    PQL_SYN_NAME getResultName();
    vector<shared_ptr<IEvaluatable>> getEvaluatables();
};
