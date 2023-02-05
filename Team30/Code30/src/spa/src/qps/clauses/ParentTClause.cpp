#include <utility>
#include <vector>

#include "ParentTClause.h"

using std::pair, std::vector;

ParentTClause::ParentTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* ParentTClause::evaluateOn() {
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

bool ParentTClause::validateArgTypes(VariableTable *variables) {
  if (left.isSynonym()
      && !variables->at(left.getSynonymName()).isStatementType()) {
    return false;
  }
  if (right.isSynonym()
      && !variables->at(right.getSynonymName()).isStatementType()) {
    return false;
  }
  return true;
}
