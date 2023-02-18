#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "arguments/ClauseArgument.h"
#include "AbstractTwoArgClause.h"

using std::shared_ptr;

class ParentClause: public AbstractTwoArgClause {
 public:
  ParentClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
