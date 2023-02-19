#include "catch.hpp"

#include <memory>

#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("FollowsClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows(1,2)
  FollowsClause followsClause = FollowsClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new StmtArgument(4)));
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(followsClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows(2,1)
  followsClause = FollowsClause(
      ClauseArgumentPtr(new StmtArgument(4)),
      ClauseArgumentPtr(new StmtArgument(1)) );
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(followsClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsClause = FollowsClause(ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})), ClauseArgumentPtr(new StmtArgument(4)));

  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
        QueryResultItem(1)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsClause = FollowsClause(ClauseArgumentPtr(new StmtArgument(1)), ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a2", QueryResultItemVector{
          QueryResultItem(2)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Both are synonyms
  followsClause = FollowsClause(ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})), ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          QueryResultItem(1)
      }},
      {"a2", QueryResultItemVector{
          QueryResultItem(2)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}

// TEMPLATE FOR CLAUSE TESTS
// Static results
// When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)

// When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
// Left arg is synonym
// Right arg is synonym
// Both sides are synonym
