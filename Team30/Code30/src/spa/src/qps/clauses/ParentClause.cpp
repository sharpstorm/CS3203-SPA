#include <utility>
#include <vector>

#include "ParentClause.h"

using std::pair, std::vector;

ParentClause::ParentClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* ParentClause::evaluateOn() {
  // Temporary implementation
  // TODO(KwanHW): Wait for pkb implementation
  StatementResult statementResult;
  statementResult.linePairs = vector<pair<int, int>>({{3, 4}, {3, 5}, {3, 6}});
  statementResult.lines = {3, 4, 5, 6};
  statementResult.isStaticTrue = false;

  QueryResult* queryResult = new QueryResult();
  if (!left.isSynonym() && !right.isSynonym()) {
    queryResult->setIsStaticTrue(true);
    return queryResult;
  }

  if (left.isSynonym()) {
    queryResult->addToStatementMap(left.getSynonymName(), statementResult);
  }

  if (right.isSynonym()) {
    queryResult->addToStatementMap(right.getSynonymName(), statementResult);
  }

  return queryResult;
}

bool ParentClause::validateArgTypes(VariableTable *variables) {
  return true;
}
