#pragma once

#include <unordered_set>

#include "Clause.h"
#include "qps/common/PQLQuerySynonymProxy.h"

using std::unordered_set;

template <class ReturnType, class RefType>
using SelectPKBGetter = unordered_set<ReturnType>(*)(
    const QueryExecutorAgent &, const RefType &);

class SelectClause : public Clause {
 private:
  PQLQuerySynonymProxy target;

  template <class ReturnType, class RefType,
      SelectPKBGetter<ReturnType, RefType> pkbGetter>
  PQLQueryResult *queryPKB(const QueryExecutorAgent &agent,
                           const PQLSynonymName &synName,
                           RefType value);

  static unordered_set<StmtValue> queryStmt(const QueryExecutorAgent &agent,
                                            const StmtRef &ref);
  static unordered_set<EntityValue> queryEntity(const QueryExecutorAgent &agent,
                                                const EntityRef &ref);


 public:
  explicit SelectClause(const PQLQuerySynonymProxy &target);
  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
  ComplexityScore getComplexityScore(const OverrideTable *table) override;
};
