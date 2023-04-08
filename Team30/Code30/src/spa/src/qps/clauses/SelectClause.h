#pragma once

#include "Clause.h"
#include "qps/common/synonym/PQLQuerySynonymProxy.h"

template<class ReturnType, class RefType>
using SelectPKBGetter = QueryResultSet<ReturnType>(*)(
    const QueryExecutorAgent &, const RefType &);

class SelectClause : public Clause {
 private:
  PQLQuerySynonymProxy target;

  template<class ReturnType, class RefType,
      SelectPKBGetter<ReturnType, RefType> pkbGetter>
  PQLQueryResult *queryPKB(const QueryExecutorAgent &agent,
                           const PQLSynonymName &synName,
                           const RefType &value) const;

  static StmtValueSet queryStmt(const QueryExecutorAgent &agent,
                                const StmtRef &ref);
  static EntityValueSet queryEntity(const QueryExecutorAgent &agent,
                                    const EntityRef &ref);

 public:
  explicit SelectClause(const PQLQuerySynonymProxy &target);
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override;
  bool validateArgTypes(const VariableTable *variables) const override;
  PQLSynonymNameList getUsedSynonyms() const override;
  ComplexityScore getComplexityScore(const OverrideTable *table) const override;
};
