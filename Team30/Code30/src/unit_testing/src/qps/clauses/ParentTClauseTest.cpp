#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/such_that/ParentClauses.h"

const ExpectedParams PARENTT_LEFT_LINES{
    {"a1", QueryResultItemVector {
        QueryResultItem(6)
    }}
};
const ExpectedParams PARENTT_RIGHT_LINES{
    {"a2", QueryResultItemVector{
        QueryResultItem(7),
        QueryResultItem(8),
        QueryResultItem(9)
    }}
};

const ExpectedParams PARENTT_PAIRS{
    {"a1", QueryResultItemVector{
        QueryResultItem(6),
        QueryResultItem(6),
        QueryResultItem(6)
    }},
    {"a2", QueryResultItemVector{
        QueryResultItem(7),
        QueryResultItem(8),
        QueryResultItem(9)
    }}
};

TEST_CASE("ParentTClause Querying") {
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

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent*(1,4)
  ParentTClause parentTClause = ParentTClause(
      make_unique<StmtArgument>(6),
      make_unique<StmtArgument>(9)
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent*(4,1)
  parentTClause = ParentTClause(
      make_unique<StmtArgument>(9),
      make_unique<StmtArgument>(6)
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentTClause = ParentTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<StmtArgument>(9));
  expected = TestQueryResultBuilder::buildExpected(PARENTT_LEFT_LINES);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentTClause = ParentTClause(
      make_unique<StmtArgument>(6),
      make_unique<SynonymArgument>(synA2)
  );
  expected = TestQueryResultBuilder::buildExpected(PARENTT_RIGHT_LINES);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentTClause = ParentTClause(
      make_unique<SynonymArgument>(synA1),
      make_unique<SynonymArgument>(synA2)
  );
  expected = TestQueryResultBuilder::buildExpected(PARENTT_PAIRS);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb.get(), override.get()));
  REQUIRE(*expected == *actual);

}
