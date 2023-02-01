#pragma once

#include <vector>
#include <memory>
#include "../../common/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"

using std::vector, std::unique_ptr;

class QueryBuilder {
 private:
  vector<QueryVariable> variables;

 public:
  QueryBuilder();
  void setResultType(PQL_VAR_TYPE type);
  void setResultVariable(PQL_VAR_NAME name);
  void addVariable(PQL_VAR_NAME name, PQL_VAR_TYPE type);
  void addSuchThat(SuchThatClause* clause);
  unique_ptr<PQLQuery> build();
};
