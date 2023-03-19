#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "../../common/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "../../common/PQLQuerySynonym.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"
#include "qps/clauses/WithClause.h"
#include "qps/clauses/WithSelectClause.h"

using std::string, std::vector, std::unique_ptr, std::unordered_map;

class QueryBuilder {
 private:
  string errorMsg;
  VariableTablePtr variables;
  vector<AttributedSynonym> resultVariables;
  vector<ClauseSPtr> clauses;
  vector<ConstraintSPtr> constraints;
  unordered_set<PQLSynonymName> declaredNames;

 public:
  QueryBuilder();
  void addResultSynonym(const AttributedSynonym &syn);
  bool hasSynonym(const PQLSynonymName &name);
  void addSynonym(const PQLSynonymName &name, const PQLSynonymType &type);
  void finalizeSynonymTable();

  PQLQuerySynonymProxy* accessSynonym(const PQLSynonymName &name);
  void addSuchThat(unique_ptr<SuchThatClause> clause);
  void addPattern(unique_ptr<PatternClause> clause);
  void addWith(unique_ptr<WithClause> clause);
  void addWithSelect(unique_ptr<WithSelectClause> clause);
  void addConstraint(ConstraintSPtr constraint);

  void setError(const string &msg);
  unique_ptr<PQLQuery> build();
};

typedef unique_ptr<QueryBuilder> QueryBuilderPtr;
