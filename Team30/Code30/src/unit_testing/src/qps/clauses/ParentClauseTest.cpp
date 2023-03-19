#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/ParentClauses.h"

//ClauseArgumentPtr(new StmtArgument(6))
//ClauseArgumentPtr(new StmtArgument(7))
//ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"}))
//ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))

// TODO(KwanHW): Multiple nesting
TEST_CASE("ParentClause Querying") {
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
  // When stmtNumLeft < stmtNumRight E.g. Parent(6,7)
  ParentClause parentClause = ParentClause(
      make_unique<StmtArgument>(6),
      make_unique<StmtArgument>(7));
  expected = PQLQueryResultPtr(new PQLQueryResult());
  actual = PQLQueryResultPtr(parentClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent(7,6)
  parentClause = ParentClause(
      make_unique<StmtArgument>(7),
      make_unique<StmtArgument>(6)
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(parentClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentClause = ParentClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<StmtArgument>(7)
      );
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
        QueryResultItem(6)
      }}
  });

  actual = PQLQueryResultPtr(parentClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentClause = ParentClause(
      make_unique<StmtArgument>(6),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a2", QueryResultItemVector{
          QueryResultItem(7)
      }}
  });

  actual = PQLQueryResultPtr(parentClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentClause = ParentClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          QueryResultItem(6)
      }},
      {"a2", QueryResultItemVector{
          QueryResultItem(7)
      }}
  });
  actual = PQLQueryResultPtr(parentClause.evaluateOn(agent));
  REQUIRE(*expected == *actual);
}
