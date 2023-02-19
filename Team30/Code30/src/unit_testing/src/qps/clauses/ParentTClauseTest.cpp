#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ParentTClause.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

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
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent*(1,4)
  ParentTClause parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new StmtArgument(9))
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent*(4,1)
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(9)),
      ClauseArgumentPtr(new StmtArgument(6))
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new StmtArgument(9)));
  expected = TestQueryResultBuilder::buildExpected(PARENTT_LEFT_LINES);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))
  );
  expected = TestQueryResultBuilder::buildExpected(PARENTT_RIGHT_LINES);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))
  );
  expected = TestQueryResultBuilder::buildExpected(PARENTT_PAIRS);
  actual = PQLQueryResultPtr(parentTClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

}
