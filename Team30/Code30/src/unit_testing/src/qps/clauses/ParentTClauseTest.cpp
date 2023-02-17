#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ParentTClause.h"
#include "ClausesPKBStub.cpp"

unordered_set<int> PARENTT_LEFT_LINES = unordered_set<int>({6});
unordered_set<int> PARENTT_RIGHT_LINES = unordered_set<int>({7, 8, 9});
pair_set<int, int> PARENTT_PAIRS = pair_set<int, int>({{6, 7}, {6, 8}, {6, 9}});

StatementResult buildParentTClauseStatementResult(bool isLeft) {
  return StatementResult{
      unordered_set<int>({isLeft ? PARENTT_LEFT_LINES : PARENTT_RIGHT_LINES}),
      PARENTT_PAIRS,
      isLeft
  };
}

//TODO(KwanHW): Multiple nesting level tests

TEST_CASE("ParentTClause Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static results
  // When stmtNumLeft < stmtNumRight E.g. Parent*(1,4)
  ParentTClause parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new StmtArgument(9))
  );
  expected = new PQLQueryResult();
  actual = parentTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // When stmtNumLeft > stmtNumRight E.g. Parent*(4,1)
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(9)),
      ClauseArgumentPtr(new StmtArgument(6))
  );
  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = parentTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Left arg is synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new StmtArgument(9)));
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentTClauseStatementResult(true));
  actual = parentTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Right arg is synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))
  );
  expected = new PQLQueryResult();
  expected->addToStatementMap("a2", buildParentTClauseStatementResult(false));
  actual = parentTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Both sides are synonym
  parentTClause = ParentTClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a1"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQLSynonymType::PQL_SYN_TYPE_ASSIGN, "a2"}))
  );
  expected = new PQLQueryResult();
  expected->addToStatementMap("a1", buildParentTClauseStatementResult(true));
  expected->addToStatementMap("a2", buildParentTClauseStatementResult(false));
  actual = parentTClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

}
