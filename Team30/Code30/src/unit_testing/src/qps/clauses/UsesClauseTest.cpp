#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/UsesClause.h"
#include "ClausesPKBStub.cpp"

using std::shared_ptr;

pair_set<int, string> USES_PAIRS({{6, "count"}, {7, "cenX"}, {7, "x"}, {8, "cenY"}, {8, "y"}});

TEST_CASE("Uses querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static Results
  UsesClause usesClause = UsesClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new EntityArgument("x")));

  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = usesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Statement synonym
  usesClause = UsesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new EntityArgument("x")));
  expected = new PQLQueryResult();
  EntityResult expectedEntityResult = EntityResult{
      unordered_set<int>({6, 7, 8}),
      unordered_set<string>(),
      USES_PAIRS,
      pair_set<string, string>(),
      true
  };
  expected->addToEntityMap("a", expectedEntityResult);
  actual = usesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);


  // Entity synonym
  usesClause = UsesClause(
      ClauseArgumentPtr(new StmtArgument(6)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));
  expectedEntityResult = EntityResult{
      unordered_set<int>(),
      unordered_set<string>({"count"}),
      pair_set<int, string>({{6, "count"}}),
      pair_set<string, string>(),
      false
  };
  expected = new PQLQueryResult();
  expected->addToEntityMap("v", expectedEntityResult);
  actual = usesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Both synonyms
  usesClause = UsesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));

  expected = new PQLQueryResult();
  EntityResult entityResult_a = EntityResult{
      unordered_set<int>({6, 7, 8}),
      unordered_set<string>(),
      USES_PAIRS,
      pair_set<string, string>(),
      true,
  };
  EntityResult entityResult_v = EntityResult{
      unordered_set<int>(),
      unordered_set<string>({"count", "cenX", "cenY", "x", "y"}),
      USES_PAIRS,
      pair_set<string, string>(),
      false,
  };
  expected->addToEntityMap("a", entityResult_a);
  expected->addToEntityMap("v", entityResult_v);
  actual = usesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);
}
