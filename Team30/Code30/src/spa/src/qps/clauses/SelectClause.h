#pragma once

#include <string>
#include "Clause.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonym target;

 public:
  explicit SelectClause(PQLQuerySynonym target);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  SynonymList getUsedSynonyms();
};
