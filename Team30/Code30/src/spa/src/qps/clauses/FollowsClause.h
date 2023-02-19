#pragma once

#include <memory>
#include <string>

#include "arguments/ClauseArgument.h"
#include "SuchThatClause.h"
#include "common/Types.h"
#include "AbstractTwoArgClause.h"

using std::shared_ptr;

class FollowsClause: public AbstractTwoArgClause {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
