#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/FollowsClauses.h"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("FollowsClause Querying") {
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
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), override.get(), cache.get());

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows(1,2)
  FollowsClause followsClause = FollowsClause(
      make_unique<StmtArgument>(1),
      make_unique<StmtArgument>(4));
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Follows(2,1)
  followsClause = FollowsClause(
      make_unique<StmtArgument>(4),
      make_unique<StmtArgument>(1));
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  followsClause = FollowsClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<StmtArgument>(4));

  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          TestResultItem(1)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  followsClause = FollowsClause(
      make_unique<StmtArgument>(1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a2", QueryResultItemVector{
          TestResultItem(2)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both are synonyms
  followsClause = FollowsClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          TestResultItem(1)
      }},
      {"a2", QueryResultItemVector{
          TestResultItem(2)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}

// TEMPLATE FOR CLAUSE TESTS
// Static results
// When stmtNumLeft < stmtNumRight E.g. Follows*(1,4)

// When stmtNumLeft > stmtNumRight E.g. Follows*(4,1)
// Left arg is synonym
// Right arg is synonym
// Both sides are synonym


TEST_CASE("FollowsClause Querying asdf") {
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
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), override.get(), cache.get());

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Follows(1,2)
  // Both are synonyms
  FollowsClause followsClause = FollowsClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          TestResultItem(1)
      }},
      {"a2", QueryResultItemVector{
          TestResultItem(2)
      }}
  });
  actual = PQLQueryResultPtr(followsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}