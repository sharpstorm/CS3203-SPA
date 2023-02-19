#pragma once

#include <memory>
#include <string>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/SuchThatClause.h"
#include "common/Types.h"
#include "qps/clauses/AbstractTwoArgClause.h"

using std::shared_ptr;

class FollowsClause: public AbstractTwoArgClause {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
