#pragma once

#include "Clause.h"
#include "qps/common/PQLQuerySynonymProxy.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonymProxy target;

 public:
  explicit SelectClause(const PQLQuerySynonymProxy &target);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};
