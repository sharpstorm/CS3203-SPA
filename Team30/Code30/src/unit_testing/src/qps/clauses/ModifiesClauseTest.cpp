#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "qps/clauses/arguments/WildcardArgument.h"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/ModifiesClause.h"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("Modifies Querying") {
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
  ModifiesClause modifiesClause = ModifiesClause(
      make_unique<StmtArgument>(1),
      make_unique<EntityArgument>("count"));

  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Wildcard - Static Statement
  modifiesClause = ModifiesClause(
      make_unique<StmtArgument>(1),
      make_unique<WildcardArgument>());
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Wildcard - Statement Synonym
  modifiesClause = ModifiesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<WildcardArgument>());
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{QueryResultItem(1), QueryResultItem(2), QueryResultItem(3), QueryResultItem(6), QueryResultItem(7), QueryResultItem(8) }}
  });
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Statement synonym
  modifiesClause = ModifiesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<EntityArgument>("count"));

  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{QueryResultItem(1), QueryResultItem(6)}}
  });
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*actual == *expected);

  // Entity synonym
  modifiesClause = ModifiesClause(
      make_unique<StmtArgument>(8),
      make_unique<SynonymArgument>(synV));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"v", QueryResultItemVector{QueryResultItem("cenY")}}
  });
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*actual == *expected);

  // Both synonyms
  modifiesClause = ModifiesClause(
      make_unique<SynonymArgument>(synA),
      make_unique<SynonymArgument>(synV));

  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{
          QueryResultItem(1),
          QueryResultItem(2),
          QueryResultItem(3),
          QueryResultItem(6),
          QueryResultItem(7),
          QueryResultItem(8)
      }},
      {"v", QueryResultItemVector {
          QueryResultItem("count"),
          QueryResultItem("cenX"),
          QueryResultItem("cenY"),
          QueryResultItem("count"),
          QueryResultItem("cenX"),
          QueryResultItem("cenY")
      }}
  });
  actual = PQLQueryResultPtr(modifiesClause.evaluateOn(agent));
  REQUIRE(*actual == *expected);
}
