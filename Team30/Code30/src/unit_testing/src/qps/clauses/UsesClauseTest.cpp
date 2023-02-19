#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/arguments/WildcardArgument.h"
#include "qps/clauses/such_that/UsesClause.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

using std::shared_ptr, std::make_unique, std::unique_ptr;

TEST_CASE("Uses querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // Static Results
  UsesClause usesClause = UsesClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new EntityArgument("x")));
  expected = make_unique<PQLQueryResult>();
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Wildcard - Static Statement
  usesClause = UsesClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new WildcardArgument()));
  expected = make_unique<PQLQueryResult>();
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Wildcard - Synonym Statement
  usesClause = UsesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new WildcardArgument()));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{QueryResultItem(6), QueryResultItem(7), QueryResultItem(8)}},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Statement synonym
  usesClause = UsesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new EntityArgument("x")));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{QueryResultItem(6), QueryResultItem(7), QueryResultItem(8)}},
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Entity synonym
  usesClause = UsesClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));
  expected = TestQueryResultBuilder::buildExpected((ExpectedParams{
      {"v", QueryResultItemVector {QueryResultItem("count")}}
  }));
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Both synonyms
  usesClause = UsesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));
//  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
//      {"a", QueryResultItemVector{QueryResultItem(6), QueryResultItem(7), QueryResultItem(8)}},
//      {"v", QueryResultItemVector{ QueryResultItem("count"), QueryResultItem("cenX"), QueryResultItem("cenY"), QueryResultItem("x"), QueryResultItem("y")}}});
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a", QueryResultItemVector{QueryResultItem(6), QueryResultItem(7), QueryResultItem(8), QueryResultItem(7), QueryResultItem(8)}},
      {"v", QueryResultItemVector {QueryResultItem("count"), QueryResultItem("cenX"), QueryResultItem("cenY"), QueryResultItem("x"), QueryResultItem("y")}}
  });
  actual = PQLQueryResultPtr(usesClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}
