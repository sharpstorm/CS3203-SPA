#pragma once

#include <memory>
#include <string>

#include "Clause.h"
#include "qps/common/AttributedSynonym.h"

using std::unique_ptr, std::string;

using WithSelectClausePredicate = bool(*)(const QueryExecutorAgent &agent,
                                          const StmtValue &stmt,
                                          const EntityValue &value);

class WithSelectClause : public Clause {
  AttributedSynonym attrSyn;
  EntityValue entVal;

  template <WithSelectClausePredicate predicate>
  void queryStmtAttributes(const QueryExecutorAgent &agent,
                           const StmtValueSet &lines,
                           StmtValueSet* output);

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
  WithSelectClause(AttributedSynonym aSyn, EntityValue entV);
  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
  ComplexityScore getComplexityScore(const OverrideTable &table) override;
};

typedef unique_ptr<WithSelectClause> WithSelectClausePtr;
