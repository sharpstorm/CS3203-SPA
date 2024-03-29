#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <utility>

#include "qps/common/synonym/PQLTypes.h"
#include "../../common/PQLQuery.h"
#include "qps/common/synonym/PQLQuerySynonym.h"
#include "../../clauses/SuchThatClause.h"
#include "../../clauses/PatternClause.h"
#include "qps/clauses/WithClause.h"
#include "qps/clauses/WithSelectClause.h"
#include "common/errors/AbstractError.h"

using std::vector, std::unique_ptr, std::unordered_map;

class QueryBuilder {
 private:
  ErrorMessage errorMsg;
  VariableTablePtr variables;
  vector<AttributedSynonym> resultVariables;
  vector<ClausePtr> clauses;
  vector<ConstraintPtr> constraints;
  unordered_set<PQLSynonymName> declaredNames;

 public:
  QueryBuilder();
  void addResultSynonym(const AttributedSynonym &syn);
  bool hasSynonym(const PQLSynonymName &name) const;
  void addSynonym(const PQLSynonymName &name,
                  const PQLSynonymType &type);
  void finalizeSynonymTable();

  PQLQuerySynonymProxy *accessSynonym(const PQLSynonymName &name);
  void addSuchThat(SuchThatClausePtr clause);
  void addPattern(PatternClausePtr clause);
  void addWith(WithClausePtr clause);
  void addWithSelect(WithSelectClausePtr clause);
  void addConstraint(ConstraintPtr constraint);

  void setError(const ErrorMessage &msg);
  PQLQueryPtr build();
};

typedef unique_ptr<QueryBuilder> QueryBuilderPtr;
