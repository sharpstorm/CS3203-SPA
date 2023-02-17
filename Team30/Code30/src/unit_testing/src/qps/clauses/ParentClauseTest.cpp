#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ParentClause.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

StatementResult buildParentClauseStatementResult(bool isLeft) {
  return StatementResult{
      unordered_set<int>({isLeft ? 6 : 7}),
      pair_set<int, int>({{6, 7}}),
      isLeft
  };
}

TEST_CASE("ParentClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent(6,7)
  ParentClause parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new StmtArgument(7))
  );
  expected = new PQLQueryResult();
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent(7,6)
  parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(7)),
      ClauseArgumentPtr(new StmtArgument(6))
  );
  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new StmtArgument(7))
      );
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentClauseStatementResult(true));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildParentClauseStatementResult(false));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentClause = ParentClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"})));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentClauseStatementResult(true));
  expected->addToStatementMap("a2", buildParentClauseStatementResult(false));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);
}
