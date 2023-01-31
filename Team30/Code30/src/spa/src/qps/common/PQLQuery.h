#pragma once

#include <string>
//#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLVariable.h"
#include "qps/clauses/Clause.h"

using std::string, std::vector;

class PQLQuery {
  VariableTable variables;
  QueryVariable resultVariable;
  vector<Clause> clauses;
  void populateVariables(vector<QueryVariable> vars);
public:
    PQLQuery(vector<QueryVariable> vars, QueryVariable resVar, vector<Clause> c);
    PQL_VAR_TYPE getSymbol(PQL_VAR_NAME name);
    vector<IEvaluatable>* getEvaluatables();
};
