#include <utility>
#include <vector>

#include "FollowsClause.h"
#include "common/Types.h"

using std::pair, std::vector;

FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* FollowsClause::evaluateOn() {
  StmtRef leftStatement = buildStatementRef(left);
  StmtRef rightStatement = buildStatementRef(right);

  // Temporary implementation
  // TODO(KwanHW): Wait for pkb implementation
  StatementResult statementResult;
  statementResult.linePairs = vector<pair<int, int>>({{1, 2}, {2, 3}, {3, 4},
                                                      {4, 5}});
  statementResult.lines = {1, 2, 3, 4, 5};
  statementResult.isStaticTrue = false;
  PQLQueryResult* queryResult = new PQLQueryResult();
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

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}

// ? Convert to Adapter from ClauseArgument to StmtRef/EntityRef
StmtRef FollowsClause::buildStatementRef(ClauseArgument argument) {
  if (argument.isStmtRef()) {
    return StmtRef{StmtType::None, argument.getStatement()};
  }

  if (argument.isWildcard()) {
    return StmtRef{StmtType::None, 0};
  }

  PQLSynonymType synType = argument.getSynonymType();
  StmtType stmtType;
  switch (synType) {
    case PQL_VAR_TYPE_STMT:
      stmtType = StmtType::Assign;
      break;
    case PQL_VAR_TYPE_READ:
      stmtType = StmtType::Read;
      break;
    case PQL_VAR_TYPE_CALL:
      stmtType = StmtType::Call;
      break;
    case PQL_VAR_TYPE_WHILE:
      stmtType = StmtType::While;
      break;
    default:
      stmtType = StmtType::None;
      break;
  }

  return StmtRef{stmtType, 0};
}
