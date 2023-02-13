#pragma once

#include <string>
#include <memory>
#include "Clause.h"
#include "ClauseArgument.h"

class SelectClause : public Clause {
 private:
  PQLQuerySynonym target;

 public:
  explicit SelectClause(PQLQuerySynonym target);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
