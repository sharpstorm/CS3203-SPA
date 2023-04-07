#include "catch.hpp"

#include <utility>

#include "../ClausesPKBStub.cpp"
#include "pkb/storage/PKB.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/executor/QueryCache.h"
#include "qps/executor/QueryExecutorAgent.h"
#include "qps/clauses/pattern/IfPatternClause.h"
#include "../../util/SynonymHolder.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "../../util/QueryResultTestUtil.cpp"

using std::move;

SynonymHolder IF_PATTERN_SYNS({{PQL_SYN_TYPE_IF, "ifs"}, {PQL_SYN_TYPE_VARIABLE, "v"}});

void testIfPattern(const QueryExecutorAgent &agent, ClauseArgumentPtr leftArg, PQLQueryResult* expected) {
  IfPatternClause clause = IfPatternClause(IF_PATTERN_SYNS.getProxy("ifs"), std::move(leftArg));
  auto actual = clause.evaluateOn(agent);
  REQUIRE(*expected == *actual);
}

void testIfPatternFalse(const QueryExecutorAgent &agent, ClauseArgumentPtr leftArg) {
  IfPatternClause clause = IfPatternClause(IF_PATTERN_SYNS.getProxy("ifs"), std::move(leftArg));
  auto actual = clause.evaluateOn(agent);
  REQUIRE(actual->isFalse());
}

TEST_CASE("If pattern clause") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  OverrideTable overrideTable;
  QueryCache overrideCache;
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);

  // Wildcard
  ClauseArgumentPtr leftArg = ClauseArgumentFactory::createWildcard();
  auto expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"ifs", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(7)
      }}
  });
  testIfPattern(agent, std::move(leftArg), expected.get());

  // Synonym
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"ifs", QueryResultItemVector{
          TestResultItem(1),
          TestResultItem(7)
      }},
      {"v", QueryResultItemVector{
        TestResultItem("x"),
        TestResultItem("y")
      }}
  });
  testIfPattern(agent, std::move(leftArg), expected.get());

  // Static - valid if and varName
  leftArg = ClauseArgumentFactory::create("x");
  expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"ifs", QueryResultItemVector{
          TestResultItem(1),
      }}
  });
  testIfPattern(agent, std::move(leftArg), expected.get());

  // Static - valid if, invalid varName
  leftArg = ClauseArgumentFactory::create("y");
  testIfPatternFalse(agent, std::move(leftArg));
}

TEST_CASE("If pattern - with Clause valid") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  QueryCache overrideCache;
  ClauseArgumentPtr leftArg;

  // Static - with Clause (valid var subbed)
  OverrideTable overrideTable;
  overrideTable.insert("v", OverrideTransformer("x"));
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  auto expected = TestQueryResultBuilder::buildExpected(ExpectedParams{
      {"ifs", QueryResultItemVector{TestResultItem(1)}},
      {"v", QueryResultItemVector{TestResultItem("x")}}
  });
  testIfPattern(agent, std::move(leftArg), expected.get());


  // Static - with Clause (valid if subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("ifs", OverrideTransformer(1));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  testIfPattern(agent, std::move(leftArg), expected.get());

  // Static - with Clause (valid if and valid var subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("ifs", OverrideTransformer(1));
  overrideTable.insert("v", OverrideTransformer("x"));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  testIfPattern(agent, std::move(leftArg), expected.get());
}

TEST_CASE("If pattern clause - with Clause invalid") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<ClausesPKBStub>(&pkb);
  ClauseArgumentPtr leftArg;
  QueryCache overrideCache;

  // Static - with Clause (invalid if subbed)
  OverrideTable overrideTable;
  overrideTable.insert("ifs", OverrideTransformer(3));
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  testIfPatternFalse(agent, std::move(leftArg));

  // Static - with Clause (invalid var subbed)
  overrideTable = OverrideTable();
  overrideTable.insert("v", OverrideTransformer("abc"));
  agent = QueryExecutorAgent(handler.get(), &overrideTable, &overrideCache);
  leftArg = ClauseArgumentFactory::create(IF_PATTERN_SYNS.getProxy("v"));
  testIfPatternFalse(agent, std::move(leftArg));
}