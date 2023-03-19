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
  
  // Static Results
  UsesClause usesClause = UsesClause(
      make_unique<StmtArgument>(1),
      make_unique<EntityArgument>("x"));
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Wildcard - Static Statement
  usesClause = UsesClause(
      make_unique<StmtArgument>(6),
      make_unique<WildcardArgument>());
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Wildcard - Synonym Statement
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<WildcardArgument>());
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
        QueryResultItem(6),
        QueryResultItem(7),
        QueryResultItem(8)
      }},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Statement synonym
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<EntityArgument>("x"));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
        QueryResultItem(6),
        QueryResultItem(7),
        QueryResultItem(8)
      }},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Entity synonym
  usesClause = UsesClause(
      make_unique<StmtArgument>(6),
      make_unique<SynonymArgument>(synV));
  expected = TestQueryResultBuilder::buildExpected((ExpectedParams{
      {"v", QueryResultItemVector {QueryResultItem("count")}}
  }));
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Both synonyms
  usesClause = UsesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<SynonymArgument>(synV));
//  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
//      {"a", QueryResultItemVector{QueryResultItem(6), QueryResultItem(7), QueryResultItem(8)}},
//      {"v", QueryResultItemVector{ QueryResultItem("count"), QueryResultItem("cenX"), QueryResultItem("cenY"), QueryResultItem("x"), QueryResultItem("y")}}});
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
          QueryResultItem(6),
          QueryResultItem(7),
          QueryResultItem(8),
          QueryResultItem(7),
          QueryResultItem(8)
      }},
      {"v", QueryResultItemVector {
          QueryResultItem("count"),
          QueryResultItem("cenX"),
          QueryResultItem("cenY"),
          QueryResultItem("x"),
          QueryResultItem("y")
      }}
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);
}
