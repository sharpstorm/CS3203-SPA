#include <utility>
#include <vector>

#include "FollowsTClause.h"

using std::pair, std::vector;

FollowsTClause::FollowsTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* FollowsTClause::evaluateOn() {
  // Temporary implementation
  // TODO(KwanHW): Wait for pkb implementation
  StatementResult statementResult;

  statementResult.linePairs = vector<pair<int, int>>({{1, 2}, {2, 3}, {3, 4},
                                                      {4, 5}, {1, 3}, {1, 4},
                                                      {1, 5}});
  statementResult.lines = {1, 2, 3, 4, 5};

  QueryResult* queryResult = new QueryResult();
  if (left.isSynonym()) {
    queryResult->addToStatementMap(left.getSynonymName(), statementResult);
  }

  if (right.isSynonym()) {
    queryResult->addToStatementMap(right.getSynonymName(), statementResult);
  }

  return queryResult;
}

bool FollowsTClause::validateArgTypes(VariableTable *variables) {
  return true;
}
