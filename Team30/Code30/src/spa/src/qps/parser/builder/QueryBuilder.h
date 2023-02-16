#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "../../common/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "../../common/PQLQuerySynonym.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"

using std::string, std::vector, std::unique_ptr, std::unordered_map;

class QueryBuilder {
 private:
  unordered_map<string, PQLQuerySynonym> variables;
  PQLQuerySynonym resultVariable;
  vector<shared_ptr<Clause>> clauses;

 public:
  QueryBuilder();
  void setResultVariable(PQLSynonymType type, PQLSynonymName name);
  bool hasVariable(PQLSynonymName name);
  void addVariable(PQLSynonymName name, PQLSynonymType type);

  PQLQuerySynonym* getVariable(PQLSynonymName name);
  void addSuchThat(unique_ptr<SuchThatClause> clause);
  void addPattern(unique_ptr<PatternClause> clause);
  unique_ptr<PQLQuery> build();
};
