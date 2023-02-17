#include "catch.hpp"

#include <memory>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "qps/clauses/arguments/StmtArgument.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ModifiesClause.h"
#include "ClausesPKBStub.cpp"
#include "qps/clauses/arguments/WildcardArgument.h"

using std::shared_ptr;

pair_set<int, string> MODIFIES_PAIRS({{1, "count"}, {2, "cenX"}, {3, "cenY"}, {6, "count"}, {7, "cenX"}, {8, "cenY"}});
TEST_CASE("Modifies Querying") {
  PKB pkbStore;
  auto pkb = shared_ptr<PkbQueryHandler>(new ClausesPKBStub(&pkbStore));

  PQLQueryResult* expected;
  PQLQueryResult* actual;

  // Static Results
  ModifiesClause modifiesClause = ModifiesClause(
     ClauseArgumentPtr(new StmtArgument(1)),
     ClauseArgumentPtr(new EntityArgument("count")));

  expected = new PQLQueryResult();
  expected->setIsStaticFalse(true);
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Wildcard - Static Statement
  modifiesClause = ModifiesClause(
      ClauseArgumentPtr(new StmtArgument(1)),
      ClauseArgumentPtr(new WildcardArgument()));
  expected = new PQLQueryResult();
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Wildcard - Statement Synonym
  modifiesClause = ModifiesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new WildcardArgument()));
  expected = new PQLQueryResult();
  EntityResult expectedEntityResult = EntityResult{
      unordered_set<int>({1, 2, 3, 6, 7, 8}),
      unordered_set<string>(),
      MODIFIES_PAIRS,
      pair_set<string, string>(),
      true
  };
  expected->addToEntityMap("a", expectedEntityResult);
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*expected == *actual);

  // Statement synonym
  modifiesClause = ModifiesClause(
     ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
     ClauseArgumentPtr(new EntityArgument("count")));

  expected = new PQLQueryResult();
  expectedEntityResult = EntityResult{
      unordered_set<int>({1, 6}),
      unordered_set<string>(),
      pair_set<int, string>({{1, "count"}, {6, "count"}}),
      pair_set<string, string>(),
      true
  };
  expected->addToEntityMap("a", expectedEntityResult);
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*actual == *expected);

  // Entity synonym
  modifiesClause = ModifiesClause(
      ClauseArgumentPtr(new StmtArgument(8)),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));

  expected = new PQLQueryResult();
  expectedEntityResult = EntityResult{
      unordered_set<int>(),
      unordered_set<string>({"cenY"}),
      pair_set<int, string>({{8, "cenY"}}),
      pair_set<string, string>(),
      false
  };
  expected->addToEntityMap("v", expectedEntityResult);
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*actual == *expected);

  // Both synonyms
  modifiesClause = ModifiesClause(
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_ASSIGN, "a"})),
      ClauseArgumentPtr(new SynonymArgument(PQLQuerySynonym{PQL_SYN_TYPE_VARIABLE, "v"})));

  expected = new PQLQueryResult();
  EntityResult entityResult_a = EntityResult{
      unordered_set<int>({1, 2, 3, 6, 7, 8}),
      unordered_set<string>(),
      MODIFIES_PAIRS,
      pair_set<string, string>(),
      true
  };
  EntityResult entityResult_v = EntityResult{
      unordered_set<int>(),
      unordered_set<string>({"count", "cenX", "cenY"}),
      MODIFIES_PAIRS,
      pair_set<string, string>(),
      false
  };
  expected->addToEntityMap("a", entityResult_a);
  expected->addToEntityMap("v", entityResult_v);
  actual = modifiesClause.evaluateOn(pkb);
  REQUIRE(*actual == *expected);
}
