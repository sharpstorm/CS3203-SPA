#pragma once

#include <memory>
#include <string>

#include "ClauseArgument.h"
#include "SuchThatClause.h"
#include "common/Types.h"

using std::shared_ptr;

class FollowsClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  FollowsClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
