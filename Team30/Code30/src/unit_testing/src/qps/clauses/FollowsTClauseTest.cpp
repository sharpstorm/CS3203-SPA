#include "catch.hpp"

#include <memory>

#include "qps/clauses/FollowsTClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

unordered_set<int> FOLLOWST_LEFT_LINES = unordered_set<int>({1, 2, 3});
unordered_set<int> FOLLOWST_RIGHT_LINES = unordered_set<int>({2, 3, 4});
pair_set<int, int> FOLLOWST_PAIRS = pair_set<int, int>({{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}});

StatementResult buildFollowsTClauseStatementResult(bool isLeft) {
  return StatementResult{
      unordered_set<int>({isLeft ? FOLLOWST_LEFT_LINES : FOLLOWST_RIGHT_LINES}),
      FOLLOWST_PAIRS,
      isLeft
  };
}

TEST_CASE("FollowsTClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)
  FollowsTClause followsTClause = FollowsTClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new StmtArgument(4)));

  expected = new PQLQueryResult();
  actual = followsTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
  followsTClause = FollowsTClause(
      ClauseArgumentPtr(new StmtArgument(4)),
      ClauseArgumentPtr(new StmtArgument(1))
      );

  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = followsTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsTClause = FollowsTClause(
      ClauseArgumentPtr (new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new StmtArgument(4)));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildFollowsTClauseStatementResult(true));
  actual = followsTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsTClause = FollowsTClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildFollowsTClauseStatementResult(false));
  actual = followsTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  followsTClause = FollowsTClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildFollowsTClauseStatementResult(true));
  expected->addToStatementMap("a2", buildFollowsTClauseStatementResult(false));
  actual = followsTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);
}
