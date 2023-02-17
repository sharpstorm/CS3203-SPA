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
  string errorMsg;
  unordered_map<string, PQLQuerySynonym> variables;
  PQLQuerySynonym resultVariable;
  vector<shared_ptr<Clause>> clauses;
  void setError(string msg);

 public:
  QueryBuilder();
  void setResultSynonym(PQLSynonymType type, PQLSynonymName name);
  bool hasSynonym(PQLSynonymName name);
  void addSynonym(PQLSynonymName name, PQLSynonymType type);

  PQLQuerySynonym* accessSynonym(PQLSynonymName name);
  void addSuchThat(unique_ptr<SuchThatClause> clause);
  void addPattern(unique_ptr<PatternClause> clause);
  unique_ptr<PQLQuery> build();
};
