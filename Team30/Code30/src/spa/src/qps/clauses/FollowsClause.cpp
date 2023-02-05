#include <unordered_set>
#include <utility>
#include <vector>

#include "FollowsClause.h"

using std::pair, std::unordered_set, std::vector;

FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* FollowsClause::evaluateOn(PkbQueryHandler pkbQueryHandler) {
  StmtRef leftStatement = buildStatementRef(left);
  StmtRef rightStatement = buildStatementRef(right);
  QueryResult<int, int> queryResult = pkbQueryHandler.queryFollows(leftStatement, rightStatement);

  PQLQueryResult*  pqlQueryResult = new PQLQueryResult();

  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticTrue(true);
    return pqlQueryResult;
  }

  StatementResult statementResult;
  PQL_VAR_NAME synonym;
  if (left.isSynonym()) {
    synonym = left.getSynonymName();
    statementResult = buildStatementResult(queryResult.firstArgVals,
                                           queryResult.pairVals, synonym);
    pqlQueryResult->addToStatementMap(synonym, statementResult);
  }

  if (right.isSynonym()) {
    synonym = right.getSynonymName();
    statementResult = buildStatementResult(queryResult.secondArgVals,
                                           queryResult.pairVals, synonym);
    pqlQueryResult->addToStatementMap(synonym, statementResult);
  }

  return pqlQueryResult;
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

StatementResult FollowsClause::buildStatementResult(unordered_set<int> linesSet,
                                                    unordered_set<pair<int, int>> pairsSet,
                                                    PQL_VAR_NAME varName) {
  vector<pair<int, int>> linePairs = vector<pair<int, int>>(pairsSet.begin(),
                                                            pairsSet.end());
  vector<int> lines = vector<int>(lines.begin(), lines.end());

  StatementResult statementResult;
  statementResult.lines = lines;
  statementResult.linePairs = linePairs;
  statementResult.isStaticTrue = false;
  return statementResult;
}