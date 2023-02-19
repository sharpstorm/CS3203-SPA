#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/such_that/ParentClause.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

using std::shared_ptr;

//ClauseArgumentPtr(new StmtArgument(6))
//ClauseArgumentPtr(new StmtArgument(7))
//ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"}))
//ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))

// TODO(KwanHW): Multiple nesting
TEST_CASE("ParentClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent(6,7)
  ParentClause parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new StmtArgument(7))
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  actual = PQLQueryResultPtr(parentClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent(7,6)
  parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(7)),
      ClauseArgumentPtr(new StmtArgument(6))
  );
  expected = PQLQueryResultPtr(new PQLQueryResult());
  expected->setIsStaticFalse(true);
  actual = PQLQueryResultPtr(parentClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new StmtArgument(7))
      );
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
        QueryResultItem(6)
      }}
  });

  actual = PQLQueryResultPtr(parentClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a2", QueryResultItemVector{
          QueryResultItem(7)
      }}
  });

  actual = PQLQueryResultPtr(parentClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"a1", QueryResultItemVector{
          QueryResultItem(6)
      }},
      {"a2", QueryResultItemVector{
          QueryResultItem(7)
      }}
  });
  actual = PQLQueryResultPtr(parentClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}
