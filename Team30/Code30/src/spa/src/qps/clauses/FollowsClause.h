#pragma once

#include <memory>
#include <string>

#include "arguments/ClauseArgument.h"
#include "SuchThatClause.h"
#include "common/Types.h"

using std::shared_ptr;

class FollowsClause: public SuchThatClause {
 private:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

 public:
  FollowsClause(ClauseArgumentPtr left,
                ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
