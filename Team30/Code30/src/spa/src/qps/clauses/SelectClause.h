#pragma once

#include "Clause.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonym target;

 public:
  explicit SelectClause(const PQLQuerySynonym &target);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};
