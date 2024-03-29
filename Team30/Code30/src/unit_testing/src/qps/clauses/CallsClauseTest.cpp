#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/arguments/WildcardArgument.h"
#include "qps/clauses/such_that/CallsClauses.h"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("AbstractEntEntClause Querying") {
  PKB pkbStore;
  auto pkb = make_unique<ClausesPKBStub>(&pkbStore);
  PQLQuerySynonym synPRaw(PQL_SYN_TYPE_PROCEDURE, "p");
  PQLQuerySynonym synQRaw(PQL_SYN_TYPE_PROCEDURE, "q");
  PQLQuerySynonym* synPRawPtr = &synPRaw;
  PQLQuerySynonym* synQRawPtr = &synQRaw;
  PQLQuerySynonymProxy synP(&synPRawPtr);
  PQLQuerySynonymProxy synQ(&synQRawPtr);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  OverrideTablePtr override = make_unique<OverrideTable>();
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), override.get(), cache.get());

  // Static results - False result
  CallsClause callsClause = CallsClause(
      make_unique<EntityArgument>("Barricade"),
      make_unique<EntityArgument>("Ironhide"));
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Static results - True result
  callsClause = CallsClause(
      make_unique<EntityArgument>("Ironhide"),
      make_unique<EntityArgument>("Barricade"));
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both syns
  callsClause = CallsClause(
      make_unique<SynonymArgument>(synP),
      make_unique<SynonymArgument>(synQ));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"p", QueryResultItemVector{TestResultItem("Bumblebee"),
                                  TestResultItem("Bumblebee"),
                                  TestResultItem("Ironhide")}},
      {"q", QueryResultItemVector{TestResultItem("Megatron"),
                                  TestResultItem("Ironhide"),
                                  TestResultItem("Barricade")}}
  });
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both wildcards
  callsClause = CallsClause(
      make_unique<WildcardArgument>(),
      make_unique<WildcardArgument>());
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Left arg is syn
  // (syn, static)
  callsClause = CallsClause(
      make_unique<SynonymArgument>(synP),
      make_unique<EntityArgument>("Barricade"));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"p", QueryResultItemVector{TestResultItem("Ironhide")}}
  });
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // (syn, wildcard)
  callsClause = CallsClause(
      make_unique<SynonymArgument>(synP),
      make_unique<WildcardArgument>());
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"p", QueryResultItemVector{TestResultItem("Bumblebee"), TestResultItem("Ironhide")}}
  });
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Right arg is syn
  // (static, syn)
  callsClause = CallsClause(
      make_unique<EntityArgument>("Bumblebee"),
      make_unique<SynonymArgument>(synQ)
  );
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"q", QueryResultItemVector{TestResultItem("Megatron"),
                                  TestResultItem("Ironhide")}}
  });
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // (wildcard, syn)
  callsClause = CallsClause(
      make_unique<WildcardArgument>(),
      make_unique<SynonymArgument>(synQ)
  );
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"q", QueryResultItemVector{TestResultItem("Megatron"),
                                  TestResultItem("Ironhide"),
                                  TestResultItem("Barricade")}}
  });
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // (static, wildcard)
  callsClause = CallsClause(
      make_unique<EntityArgument>("Bumblebee"),
      make_unique<WildcardArgument>()
  );
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // (wildcard, static)
  callsClause = CallsClause(
      make_unique<WildcardArgument>(),
      make_unique<EntityArgument>("Megatron")
  );
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(callsClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}
