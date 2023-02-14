#include "catch.hpp"

#include <memory>

#include "qps/clauses/ParentClause.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

ClauseArgument PARENT_LEFT_STATIC_ARG{6};
ClauseArgument PARENT_RIGHT_STATIC_ARG{7};
ClauseArgument PARENT_LEFT_ARG{PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"}};
ClauseArgument PARENT_RIGHT_ARG{PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}};

StatementResult buildParentClauseStatementResult(bool isLeft) {
  return StatementResult{
      unordered_set<int>({isLeft ? 6 : 7}),
      pair_set<int, int>({{6, 7}}),
      isLeft
  };
}

// TODO(KwanHW): Multiple nesting
TEST_CASE("ParentClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent(6,7)
  ParentClause parentClause = ParentClause(PARENT_LEFT_STATIC_ARG, PARENT_RIGHT_STATIC_ARG);
  expected = new PQLQueryResult();
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent(7,6)
  parentClause = ParentClause(PARENT_RIGHT_STATIC_ARG, PARENT_LEFT_STATIC_ARG);
  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentClause = ParentClause(PARENT_LEFT_ARG, PARENT_RIGHT_STATIC_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentClauseStatementResult(true));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentClause = ParentClause(PARENT_LEFT_STATIC_ARG, PARENT_RIGHT_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildParentClauseStatementResult(false));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentClause = ParentClause(PARENT_LEFT_ARG, PARENT_RIGHT_ARG);
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentClauseStatementResult(true));
  expected->addToStatementMap("a2", buildParentClauseStatementResult(false));
  actual = parentClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);
}
