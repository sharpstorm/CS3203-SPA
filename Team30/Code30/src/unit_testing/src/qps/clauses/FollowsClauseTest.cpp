#include "catch.hpp"

#include <memory>

#include "qps/clauses/FollowsClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

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
  FollowsClause followsClause = FollowsClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new StmtArgument(4)));
  expected = new PQLQueryResult();
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows(2,1)
  followsClause = FollowsClause(
      ClauseArgumentPtr(new StmtArgument(4)),
      ClauseArgumentPtr(new StmtArgument(1)) );
  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsClause = FollowsClause(ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})), ClauseArgumentPtr(new StmtArgument(4)));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildFollowsClauseStatementResult(true));
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsClause = FollowsClause(ClauseArgumentPtr(new StmtArgument(1)), ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildFollowsClauseStatementResult(false));
  actual = followsClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);


  // Both are synonyms
  followsClause = FollowsClause(ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})), ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
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
