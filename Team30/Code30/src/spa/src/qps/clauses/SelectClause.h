#pragma once

#include <string>
#include "Clause.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonym target;

 public:
  explicit SelectClause(const PQLQuerySynonym &target);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};
