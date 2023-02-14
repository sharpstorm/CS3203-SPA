#include "catch.hpp"

#include <memory>

#include "qps/clauses/ClauseArgument.h"
#include "qps/clauses/FollowsClause.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

ClauseArgument FOLLOWS_LEFT_STATIC_ARG{1};
ClauseArgument FOLLOWS_RIGHT_STATIC_ARG{2};
ClauseArgument FOLLOWS_LEFT_ARG{PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"}};
ClauseArgument FOLLOWS_RIGHT_ARG{PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}};

StatementResult buildFollowsClauseStatementResult(bool isLeft) {
  return StatementResult{
      unordered_set<int>({isLeft ? 1 : 2}),
      pair_set<int, int>({{1, 2}}),
      isLeft
  };
}

TEST_CASE("FollowsClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows(1,2)
  FollowsClause followsClause = FollowsClause(FOLLOWS_LEFT_STATIC_ARG, FOLLOWS_RIGHT_STATIC_ARG);
  expected = new PQLQueryResult();
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows(2,1)
  followsClause = FollowsClause(FOLLOWS_RIGHT_STATIC_ARG, FOLLOWS_LEFT_STATIC_ARG);
  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsClause = FollowsClause(FOLLOWS_LEFT_ARG, FOLLOWS_RIGHT_STATIC_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildFollowsClauseStatementResult(true));
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsClause = FollowsClause(FOLLOWS_LEFT_STATIC_ARG, FOLLOWS_RIGHT_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildFollowsClauseStatementResult(false));
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);


  // Both are synonyms
  followsClause = FollowsClause(FOLLOWS_LEFT_ARG, FOLLOWS_RIGHT_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildFollowsClauseStatementResult(true));
  expected->addToStatementMap("a2", buildFollowsClauseStatementResult(false));
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);
}

// TEMPLATE FOR CLAUSE TESTS
// Static results
// When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)

// When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
// Left arg is synonym
// Right arg is synonym
// Both sides are synonym
