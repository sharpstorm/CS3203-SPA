#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "../../common/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "../../common/PQLQueryVariable.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"

using std::string, std::vector, std::unique_ptr, std::unordered_map;

class QueryBuilder {
 private:
  unordered_map<string, PQLQueryVariable> variables;
  PQLQueryVariable resultVariable;
  vector<shared_ptr<Clause>> clauses;
 public:
  QueryBuilder();
  void setResultType(PQLSynonymType type);
  void setResultVariable(PQL_VAR_NAME name);
  bool hasVariable(PQL_VAR_NAME name);
  void addVariable(PQL_VAR_NAME name, PQLSynonymType type);
  PQLSynonymType* getVariableType(PQL_VAR_NAME name);
  PQLQueryVariable* getVariable(PQL_VAR_NAME name);
  void addSuchThat(unique_ptr<SuchThatClause> clause);
  unique_ptr<PQLQuery> build();
};
