#pragma once

#include <memory>

#include "Clause.h"
#include "qps/common/synonym/AttributedSynonym.h"

using std::unique_ptr;

using WithSelectClausePredicate = bool (*)(const QueryExecutorAgent &agent,
                                           const StmtValue &stmt,
                                           const EntityValue &value);

class WithSelectClause : public Clause {
  AttributedSynonym attrSyn;
  EntityValue entVal;

  template<WithSelectClausePredicate predicate>
  void queryStmtAttributes(const QueryExecutorAgent &agent,
                           const StmtValueSet &lines,
                           StmtValueSet *output) const;
  void projectAttribute(StmtValueSet *output,
                        const StmtValueSet &targets,
                        const QueryExecutorAgent &agent) const;

  static inline bool isPrintVarName(const QueryExecutorAgent &agent,
                                    const StmtValue &stmt,
                                    const EntityValue &value);
  static inline bool isReadVarName(const QueryExecutorAgent &agent,
                                   const StmtValue &stmt,
                                   const EntityValue &value);
  static inline bool isCallProcName(const QueryExecutorAgent &agent,
                                    const StmtValue &stmt,
                                    const EntityValue &value);

 public:
  WithSelectClause(const AttributedSynonym &aSyn, const EntityValue &entV);
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override;
  bool validateArgTypes() const override;
  PQLSynonymNameList getUsedSynonyms() const override;
  ComplexityScore getComplexityScore(const OverrideTable *table) const override;
};

typedef unique_ptr<WithSelectClause> WithSelectClausePtr;
