#pragma once

#include <string>
#include "Clause.h"
#include "ClauseArgument.h"

class SelectClause : public Clause {
 private:
  PQLQueryVariable target;

 public:
  SelectClause(PQLQueryVariable target);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
