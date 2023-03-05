#include "catch.hpp"

#include <memory>

#include "qps/clauses/such_that/FollowsTClause.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

using std::shared_ptr, std::make_unique, std::unique_ptr;

const ExpectedParams FOLLOWST_LEFT_LINES{
    {"a1", QueryResultItemVector {
        QueryResultItem(1),
        QueryResultItem(2),
        QueryResultItem(3)
    }}
};
const ExpectedParams FOLLOWST_RIGHT_LINES{
    {"a2", QueryResultItemVector{
        QueryResultItem(2),
        QueryResultItem(3),
        QueryResultItem(4)
    }}
};

const ExpectedParams FOLLOWST_PAIRS{
    {"a1", QueryResultItemVector{
        QueryResultItem(1),
        QueryResultItem(1),
        QueryResultItem(1),
        QueryResultItem(2),
        QueryResultItem(2),
        QueryResultItem(3)
    }},
    {"a2", QueryResultItemVector{
        QueryResultItem(2),
        QueryResultItem(3),
        QueryResultItem(4),
        QueryResultItem(3),
        QueryResultItem(4),
        QueryResultItem(4)
    }}
};

TEST_CASE("FollowsTClause Querying") {
  PKB pkbStore;
  auto pkb = make_unique<ClausesPKBStub>(&pkbStore);
  PQLQuerySynonym synA1(PQL_SYN_TYPE_ASSIGN, "a1");
  PQLQuerySynonym synA2(PQL_SYN_TYPE_ASSIGN, "a2");

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)
  FollowsTClause followsTClause = FollowsTClause(
      make_unique<StmtArgument>(1),
  make_unique<StmtArgument>(4));

  expected = PQLQueryResultPtr(new PQLQueryResult());
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(pkb.get()));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
  followsTClause = FollowsTClause(
      make_unique<StmtArgument>(4),
      make_unique<StmtArgument>(1));

  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(pkb.get()));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsTClause = FollowsTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<StmtArgument>(4));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_LEFT_LINES);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(pkb.get()));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsTClause = FollowsTClause(
      make_unique<StmtArgument>(1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_RIGHT_LINES);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(pkb.get()));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  followsTClause = FollowsTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(FOLLOWST_PAIRS);
  actual = PQLQueryResultPtr(followsTClause.evaluateOn(pkb.get()));
  REQUIRE(*expected == *actual);
}
