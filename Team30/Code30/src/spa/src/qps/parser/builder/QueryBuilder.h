#pragma once

#include <vector>
#include "../../common/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"

class QueryBuilder {
    QueryVariable qVar;
    vector<QueryVariable> vars;
    vector<Clause> clauses;
 public:
  QueryBuilder();
  void setResultType(PQL_VAR_TYPE type);
  void setResultVariable(PQL_VAR_NAME name);
  void addVariable(PQL_VAR_NAME name, PQL_VAR_TYPE type);
  void addSuchThat(SuchThatClause* clause);
  PQLQuery* build();
};
