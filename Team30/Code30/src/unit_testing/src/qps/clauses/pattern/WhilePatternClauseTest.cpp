#include "catch.hpp"

#include <utility>

#include "../ClausesPKBStub.cpp"
#include "pkb/storage/PKB.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/executor/QueryCache.h"
#include "qps/executor/QueryExecutorAgent.h"
#include "qps/clauses/pattern/WhilePatternClause.h"
#include "../../util/SynonymHolder.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "../../util/QueryResultTestUtil.cpp"

SynonymHolder WHILE_PATTERN_SYNS({{PQL_SYN_TYPE_WHILE, "w"}, {PQL_SYN_TYPE_VARIABLE, "v"}});

void testWhilePattern(const QueryExecutorAgent &agent, ClauseArgumentPtr leftArg, PQLQueryResult* expected) {
  WhilePatternClause clause = WhilePatternClause(WHILE_PATTERN_SYNS.getProxy("w"), std::move(leftArg));
  auto actual = clause.evaluateOn(agent);
  REQUIRE(*expected == *actual);
}

void testWhilePatternFalse(const QueryExecutorAgent &agent, ClauseArgumentPtr leftArg) {
  WhilePatternClause clause = WhilePatternClause(WHILE_PATTERN_SYNS.getProxy("w"), std::move(leftArg));
  auto actual = clause.evaluateOn(agent);
  REQUIRE(actual->isFalse());
}

TEST_CASE("While pattern clause") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  OverrideTable overrideTable;
  QueryCache overrideCache;
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);

  // Wildcard
  ClauseArgumentPtr leftArg = ClauseArgumentFactory::createWildcard();
  auto expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"w", QueryResultItemVector{
          TestResultItem(4),
          TestResultItem(11)
      }}
  });
  testWhilePattern(agent, std::move(leftArg), expected.get());

  // Synonym
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"w", QueryResultItemVector{
          TestResultItem(4),
          TestResultItem(11)
      }},
      {"v", QueryResultItemVector{
        TestResultItem("x"),
        TestResultItem("y")
      }}
  });
  testWhilePattern(agent, std::move(leftArg), expected.get());

  // Static - valid varName
  leftArg = ClauseArgumentFactory::create("x");
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"w", QueryResultItemVector{
          TestResultItem(4),
      }}
  });
  testWhilePattern(agent, std::move(leftArg), expected.get());

  // Static - invalid varName
  leftArg = ClauseArgumentFactory::create("g");
  testWhilePatternFalse(agent, std::move(leftArg));
}

TEST_CASE("While pattern - with Clause valid") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  QueryCache overrideCache;
  ClauseArgumentPtr leftArg;
  auto expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"w", QueryResultItemVector{TestResultItem(4)}},
      {"v", QueryResultItemVector{TestResultItem("x")}}
  });

  // Static - with Clause (valid var subbed)
  OverrideTable overrideTable;
  overrideTable.insert("v", OverrideTransformer("x"));
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  testWhilePattern(agent, std::move(leftArg), expected.get());

  // Static - with Clause (valid if subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("w", OverrideTransformer(4));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  testWhilePattern(agent, std::move(leftArg), expected.get());

  // Static - with Clause (valid if and valid var subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("v", OverrideTransformer("x"));
  overrideTable.insert("w", OverrideTransformer(4));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  testWhilePattern(agent, std::move(leftArg), expected.get());
}

TEST_CASE("While pattern - with Clause invalid") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  ClauseArgumentPtr leftArg;
  QueryCache overrideCache;

  // Static - with Clause (invalid while subbed)
  OverrideTable overrideTable;
  overrideTable.insert("w", OverrideTransformer(3));
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  testWhilePatternFalse(agent, std::move(leftArg));

  // Static - with Clause (invalid var subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("v", OverrideTransformer("abc"));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(WHILE_PATTERN_SYNS.getProxy("v"));
  testWhilePatternFalse(agent, std::move(leftArg));
}

