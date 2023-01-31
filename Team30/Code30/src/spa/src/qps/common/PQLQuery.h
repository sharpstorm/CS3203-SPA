#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLVariable.h"
#include "qps/clauses/Clause.h"

using std::string, std::unordered_map, std::vector;

//struct QueryVariable {
//  PQL_VAR_TYPE type;
//  PQL_VAR_NAME name;
//};
//
//typedef unordered_map<string, QueryVariable> VariableTable;

class PQLQuery {
  VariableTable variables;
  QueryVariable resultVariable;
  vector<Clause> clauses;
  void populateVariables(vector<QueryVariable> vars);
public:
    PQLQuery(vector<QueryVariable> vars, QueryVariable resVar, vector<Clause> c);
//    void addSymbol(PQL_VAR_NAME name,PQL_VAR_TYPE type);
    PQL_VAR_TYPE getSymbol(PQL_VAR_NAME name);
    vector<IEvaluatable>* getEvaluatables();
};
