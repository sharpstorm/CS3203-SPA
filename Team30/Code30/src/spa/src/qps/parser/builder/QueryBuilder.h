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
  vector<PQLQuerySynonym> resultVariables;
  vector<ClauseSPtr> clauses;

 public:
  QueryBuilder();
  void addResultSynonym(const PQLQuerySynonym &synonym);
  bool hasSynonym(const PQLSynonymName &name);
  void addSynonym(const PQLSynonymName &name, const PQLSynonymType &type);

  PQLQuerySynonym* accessSynonym(const PQLSynonymName &name);
  void addSuchThat(unique_ptr<SuchThatClause> clause);
  void addPattern(unique_ptr<PatternClause> clause);

  void setError(const string &msg);
  unique_ptr<PQLQuery> build();
};

typedef unique_ptr<QueryBuilder> QueryBuilderPtr;
