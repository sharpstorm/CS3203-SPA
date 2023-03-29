#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/FollowsClauses.h"

using std::shared_ptr, std::make_unique, std::unique_ptr;

const ExpectedParams FOLLOWST_LEFT_LINES{
    {"a1", QueryResultItemVector {
        TestResultItem(1),
        TestResultItem(2),
        TestResultItem(3)
    }}
};
const ExpectedParams FOLLOWST_RIGHT_LINES{
    {"a2", QueryResultItemVector{
        TestResultItem(2),
        TestResultItem(3),
        TestResultItem(4)
    }}
};

const ExpectedParams FOLLOWST_PAIRS{
    {"a1", QueryResultItemVector{
        TestResultItem(1),
        TestResultItem(1),
        TestResultItem(1),
        TestResultItem(2),
        TestResultItem(2),
        TestResultItem(3)
    }},
    {"a2", QueryResultItemVector{
        TestResultItem(2),
        TestResultItem(3),
        TestResultItem(4),
        TestResultItem(3),
        TestResultItem(4),
        TestResultItem(4)
    }}
};

TEST_CASE("FollowsTClause Querying") {
  PKB pkbStore;
  auto pkb = make_unique<ClausesPKBStub>(&pkbStore);
  PQLQuerySynonym synA1Raw(PQL_SYN_TYPE_ASSIGN, "a1");
  PQLQuerySynonym synA2Raw(PQL_SYN_TYPE_ASSIGN, "a2");
  PQLQuerySynonym* synA1Ptr = &synA1Raw;
  PQLQuerySynonym* synA2Ptr = &synA2Raw;
  PQLQuerySynonymProxy synA1(&synA1Ptr);
  PQLQuerySynonymProxy synA2(&synA2Ptr);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  OverrideTablePtr override = make_unique<OverrideTable>();
  QueryExecutorAgent agent(pkb.get(), override.get());
  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)
  FollowsTClause followsTClause = FollowsTClause(
      make_unique<StmtArgument>(1),
  make_unique<StmtArgument>(4));

  expected = PQLQueryResultPtr(new PQLQueryResult());

  actual = PQLQueryResultPtr(followsTClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
  followsTClause = FollowsTClause(
      make_unique<StmtArgument>(4),
      make_unique<StmtArgument>(1));

  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsTClause = FollowsTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<StmtArgument>(4));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_LEFT_LINES);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsTClause = FollowsTClause(
      make_unique<StmtArgument>(1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_RIGHT_LINES);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  followsTClause = FollowsTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_PAIRS);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}
