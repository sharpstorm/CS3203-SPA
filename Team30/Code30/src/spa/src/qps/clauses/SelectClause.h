#pragma once

#include "Clause.h"
#include "qps/common/PQLQuerySynonymProxy.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonymProxy target;

 public:
  explicit SelectClause(const PQLQuerySynonymProxy &target);
  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};
