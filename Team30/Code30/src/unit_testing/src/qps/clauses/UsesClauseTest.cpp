#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/arguments/WildcardArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/UsesClause.h"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("Uses querying") {
  PKB pkbStore;
  auto pkb = make_unique<ClausesPKBStub>(&pkbStore);
  PQLQuerySynonym assignSynRaw(PQL_SYN_TYPE_ASSIGN, "a");
  PQLQuerySynonym varSynRaw(PQL_SYN_TYPE_VARIABLE, "v");
  PQLQuerySynonym* assignSynPtr = &assignSynRaw;
  PQLQuerySynonym* varSynPtr = &varSynRaw;
  PQLQuerySynonymProxy synA(&assignSynPtr);
  PQLQuerySynonymProxy synV(&varSynPtr);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  OverrideTablePtr override = make_unique<OverrideTable>();
  QueryExecutorAgent agent(pkb.get(), override.get());
  
  // Static Results
  UsesClause usesClause = UsesClause(
      make_unique<StmtArgument>(1),
      make_unique<EntityArgument>("x"));
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Wildcard - Static Statement
  usesClause = UsesClause(
      make_unique<StmtArgument>(6),
      make_unique<WildcardArgument>());
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Wildcard - Synonym Statement
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<WildcardArgument>());
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
        TestResultItem(6),
        TestResultItem(7),
        TestResultItem(8)
      }},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Statement synonym
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<EntityArgument>("x"));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
        TestResultItem(6),
        TestResultItem(7),
        TestResultItem(8)
      }},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Entity synonym
  usesClause = UsesClause(
      make_unique<StmtArgument>(6),
      make_unique<SynonymArgument>(synV));
  expected = TestQueryResultBuilder::buildExpected((ExpectedParams{
      {"v", QueryResultItemVector {TestResultItem("count")}}
  }));
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both synonyms
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<SynonymArgument>(synV));
//  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
//      {"a", QueryResultItemVector{TestResultItem(6), TestResultItem(7), TestResultItem(8)}},
//      {"v", QueryResultItemVector{ TestResultItem("count"), TestResultItem("cenX"), TestResultItem("cenY"), TestResultItem("x"), TestResultItem("y")}}});
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
          TestResultItem(6),
          TestResultItem(7),
          TestResultItem(8),
          TestResultItem(7),
          TestResultItem(8)
      }},
      {"v", QueryResultItemVector {
          TestResultItem("count"),
          TestResultItem("cenX"),
          TestResultItem("cenY"),
          TestResultItem("x"),
          TestResultItem("y")
      }}
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}
