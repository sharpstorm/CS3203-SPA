#include "catch.hpp"
#include "qps/common/PQLQuery.h"
#include "qps/parser/QueryParser.h"
#include "../SourceParserStub.h"
#include "qps/executor/planner/QueryPlan.h"
#include "qps/executor/planner/QueryPlanner.h"
#include "qps/clauses/pattern/IfPatternClause.h"
#include "../../TestUtils.h"

template <class ClauseType>
ClauseType* assertIsClass(IEvaluatable* ptr) {
  auto fc = dynamic_cast<ClauseType*>(ptr);
  REQUIRE(fc != nullptr);
  return fc;
}

template <class ClauseType1, class ClauseType2>
void assertIsEitherClass(IEvaluatable* ptr) {
  auto c1 = dynamic_cast<ClauseType1*>(ptr);
  auto c2 = dynamic_cast<ClauseType2*>(ptr);
  REQUIRE(((c1 != nullptr) || (c2 != nullptr)));
}

template <class T, class U>
void executeClauseCompare(string query, vector<string> inputOverrides,
                          string synFirst, string synSecond) {
  SourceParserStub exprParser;
  QueryParser parser(&exprParser);
  QueryPlanner planner;

  PQLQueryPtr pqlQuery = parser.parseQuery(&query);
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();
  for (auto x : inputOverrides) {
    overrideTable->insert(x, OverrideTransformer(2));
  }

  QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(), overrideTable.get());
  REQUIRE(!plan->isEmpty());
  REQUIRE(plan->getGroupCount() == 1);

  auto group = plan->getGroup(0);
  REQUIRE(!group->canBeEmpty());

  auto clauses = group->getConditionalClauses();
  auto r1 = assertIsClass<T>(clauses.at(0));
  auto r2 = assertIsClass<U>(clauses.at(1));

  REQUIRE(TestUtils::vectorContains(r1->getUsedSynonyms(), string(synFirst)));
  REQUIRE(TestUtils::vectorContains(r2->getUsedSynonyms(), string(synSecond)));
}

typedef string(*TemplateGenerator)(const string &input);

TEST_CASE("Prefer constants on all except Uses, Modifies Clauses") {
  vector<TemplateGenerator> templates = {
      [](const string &relation){
        return "stmt s1, s2; Select BOOLEAN such that "
            + relation + "(1, 2) and "
            + relation + "(s1, 2) and "
            + relation + "(1, s1) and "
            + relation + "(s1, s2)";
      },
      [](const string &relation){
        return "procedure p1, p2; Select BOOLEAN such that "
            + relation + "(\"x\", \"y\") and "
            + relation + "(p1, \"y\") and "
            + relation + "(\"x\", p1) and "
            + relation + "(p1, p2)";
      },
      [](const string &relation){
        return "stmt s1; variable v1; Select BOOLEAN such that "
            + relation + "(1, \"x\") and "
            + relation + "(s1, \"x\") and "
            + relation + "(1, v1) and "
            + relation + "(s1, v1)";
      },
      [](const string &relation){
        return "procedure p1; variable v1; Select BOOLEAN such that "
            + relation + "(\"proc\", \"x\") and "
            + relation + "(p1, \"x\") and "
            + relation + "(\"proc\", v1) and "
            + relation + "(p1, v1)";
      }
  };

  vector<pair<string, int>> cases = {
      { "Follows", 0 },
      { "Follows*", 0 },
      { "Parent", 0 },
      { "Parent*", 0 },
      { "Next", 0 },
      { "Next*", 0 },
      { "Affects", 0 },
      { "Affects*", 0 },
      { "Calls", 1 },
      { "Calls*", 1 },
  };

  for (auto c : cases) {
    auto relation = c.first;
    auto format = c.second;
    string query = templates.at(format)(relation);

    SourceParserStub exprParser;
    QueryParser parser(&exprParser);
    QueryPlanner planner;

    PQLQueryPtr pqlQuery = parser.parseQuery(&query);
    OverrideTablePtr overrideTable = make_unique<OverrideTable>();
    QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(),
                                                 overrideTable.get());

    REQUIRE(!plan->isEmpty());
    REQUIRE(plan->getGroupCount() == 2);

    auto groupConst = plan->getGroup(0);
    auto groupQuery = plan->getGroup(1);
    REQUIRE(!groupConst->canBeEmpty());
    REQUIRE(!groupQuery->canBeEmpty());

    auto clauses = groupConst->getConditionalClauses();
    auto r1 = assertIsClass<AbstractTwoArgClause>(clauses.at(0));

    clauses = groupQuery->getConditionalClauses();
    auto r2 = assertIsClass<AbstractTwoArgClause>(clauses.at(0));
    auto r3 = assertIsClass<AbstractTwoArgClause>(clauses.at(1));
    auto r4 = assertIsClass<AbstractTwoArgClause>(clauses.at(2));

    REQUIRE(r1->getUsedSynonyms().size() == 0);
    REQUIRE(r2->getUsedSynonyms().size() == 1);
    REQUIRE(r3->getUsedSynonyms().size() == 1);
    REQUIRE(r4->getUsedSynonyms().size() == 2);
  }
}

TEST_CASE("Prefer constants on Uses, Modifies Clauses") {
  vector<TemplateGenerator> templates = {
      [](const string &relation){
        return "stmt s1; variable v1; Select BOOLEAN such that "
            + relation + "(1, \"x\") and "
            + relation + "(s1, \"x\") and "
            + relation + "(1, v1) and "
            + relation + "(s1, v1)";
      },
      [](const string &relation){
        return "procedure p1; variable v1; Select BOOLEAN such that "
            + relation + "(\"proc\", \"x\") and "
            + relation + "(p1, \"x\") and "
            + relation + "(\"proc\", v1) and "
            + relation + "(p1, v1)";
      }
  };

  vector<pair<string, int>> cases = {
      { "Uses", 0 },
      { "Uses", 1 },
      { "Modifies", 0 },
      { "Modifies", 1 },
  };

  for (auto c : cases) {
    auto relation = c.first;
    auto format = c.second;
    string query = templates.at(format)(relation);

    SourceParserStub exprParser;
    QueryParser parser(&exprParser);
    QueryPlanner planner;

    PQLQueryPtr pqlQuery = parser.parseQuery(&query);
    OverrideTablePtr overrideTable = make_unique<OverrideTable>();
    QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(),
                                                 overrideTable.get());

    REQUIRE(!plan->isEmpty());
    REQUIRE(plan->getGroupCount() == 2);

    auto groupConst = plan->getGroup(0);
    auto groupQuery = plan->getGroup(1);
    REQUIRE(!groupConst->canBeEmpty());
    REQUIRE(!groupQuery->canBeEmpty());

    auto clauses = groupConst->getConditionalClauses();
    auto r1 = assertIsClass<AbstractTwoArgClause>(clauses.at(0));

    clauses = groupQuery->getConditionalClauses();
    auto r2 = assertIsClass<AbstractTwoArgClause>(clauses.at(0));
    auto r3 = assertIsClass<AbstractTwoArgClause>(clauses.at(1));
    auto r4 = assertIsClass<AbstractTwoArgClause>(clauses.at(2));

    REQUIRE(r1->getUsedSynonyms().size() == 0);
    REQUIRE(r2->getUsedSynonyms().size() == 1);
    REQUIRE(r3->getUsedSynonyms().size() == 2);
    REQUIRE(r4->getUsedSynonyms().size() == 1);
  }
}

TEST_CASE("Test Prefer With, Pattern, Static Such That, Compute Such That") {
  string query = "assign s1; if ifs1; constant c; Select BOOLEAN "
                 "such that Follows*(ifs1, s1) and Next*(s1, ifs1)"
                 "pattern ifs1(_,_,_)"
                 "with ifs1.stmt# = c.value";
  SourceParserStub exprParser;
  QueryParser parser(&exprParser);
  QueryPlanner planner;

  PQLQueryPtr pqlQuery = parser.parseQuery(&query);
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();

  QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(), overrideTable.get());
  REQUIRE(!plan->isEmpty());
  REQUIRE(plan->getGroupCount() == 1);

  auto group = plan->getGroup(0);
  REQUIRE(!group->canBeEmpty());

  auto clauses = group->getConditionalClauses();
  assertIsClass<WithClause>(clauses.at(0));
  assertIsClass<IfPatternClause>(clauses.at(1));
  assertIsClass<FollowsTClause>(clauses.at(2));
  assertIsClass<NextTClause>(clauses.at(3));
}

TEST_CASE("Test Prefer Overridden") {
  executeClauseCompare<FollowsClause, FollowsClause>(
      "stmt s1, s2, s3; Select BOOLEAN "
      "such that Follows(s1, s2) and Follows(s1, s3)"
      "with s3.stmt# = 2",
      {"s3"},
      "s3", "s2");
  executeClauseCompare<FollowsClause, FollowsClause>(
      "stmt s1, s2, s3; Select BOOLEAN "
      "such that Follows(s1, s3) and Follows(s1, s2)"
      "with s3.stmt# = 2",
      {"s3"},
      "s3", "s2");
}

TEST_CASE("Test Prefer Tighter Syn") {
  executeClauseCompare<FollowsClause, FollowsClause>(
      "stmt s1, s2; print p1, p2; Select BOOLEAN "
      "such that Follows(s1, s2) and Follows(s1, p1)",
      {},
      "p1", "s2");
  executeClauseCompare<FollowsClause, FollowsClause>(
      "stmt s1, s2; print p1, p2; Select BOOLEAN "
      "such that Follows(s1, p1) and Follows(s1, s2)",
      {},
      "p1", "s2");


  executeClauseCompare<FollowsClause, FollowsClause>(
      "if i1, i2; print p1, p2; Select BOOLEAN "
      "such that Follows(p1, i1) and Follows(p1, p2)",
      {},
      "i1", "p2");

  executeClauseCompare<FollowsClause, FollowsClause>(
      "if i1, i2; print p1, p2; Select BOOLEAN "
      "such that Follows(p1, p2) and Follows(p1, i1)",
      {},
      "i1", "p2");

  executeClauseCompare<FollowsClause, FollowsClause>(
      "if i1, i2; print p1, p2; Select BOOLEAN "
      "such that Follows(p1, p2) and Follows(p1, i1)",
      {},
      "i1", "p2");
}

TEST_CASE("Test Prefer Non Transitive") {
  executeClauseCompare<FollowsClause, FollowsTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Follows(s1, s2) and Follows*(s1, s2)",
      {},
      "s1", "s1");

  executeClauseCompare<FollowsClause, FollowsTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Follows*(s1, s2) and Follows(s1, s2)",
      {},
      "s1", "s1");
}

TEST_CASE("Test Prefer Follows* over Next*") {
  executeClauseCompare<FollowsTClause, NextTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Follows*(s1, s2) and Next*(s1, s2)",
      {},
      "s1", "s1");

  executeClauseCompare<FollowsTClause, NextTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Next*(s1, s2) and Follows*(s1, s2)",
      {},
      "s1", "s1");
}

TEST_CASE("Test Prefer Affects over Next*") {
  executeClauseCompare<AffectsClause, NextTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Affects(s1, s2) and Next*(s1, s2)",
      {},
      "s1", "s1");

  executeClauseCompare<AffectsClause, NextTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Next*(s1, s2) and Affects(s1, s2)",
      {},
      "s1", "s1");
}

TEST_CASE("Test Prefer Affects over Affects*") {
  executeClauseCompare<AffectsClause, AffectsTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Affects(s1, s2) and Affects*(s1, s2)",
      {},
      "s1", "s1");

  executeClauseCompare<AffectsClause, AffectsTClause>(
      "stmt s1, s2; Select BOOLEAN "
      "such that Affects*(s1, s2) and Affects(s1, s2)",
      {},
      "s1", "s1");
}

TEST_CASE("Test Prefer Deferred Compute Heavy Clauses") {
  string query = "stmt s1, s2, s3, s4, s5; constant c; Select BOOLEAN "
                 "such that Follows(s1, s2) and Affects(s1, s5) "
                 "and Follows(s2, s3) and Follows(s3, s4) "
                 "with s5.stmt# = c.value";

  SourceParserStub exprParser;
  QueryParser parser(&exprParser);
  QueryPlanner planner;

  PQLQueryPtr pqlQuery = parser.parseQuery(&query);
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();
  QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(),
                                               overrideTable.get());

  REQUIRE(!plan->isEmpty());
  REQUIRE(plan->getGroupCount() == 1);

  auto group = plan->getGroup(0);
  REQUIRE(!group->canBeEmpty());

  auto clauses = group->getConditionalClauses();
  assertIsClass<FollowsClause>(clauses.at(0));
  assertIsClass<FollowsClause>(clauses.at(1));
  assertIsClass<FollowsClause>(clauses.at(2));
  assertIsClass<AffectsClause>(clauses.at(3));
  assertIsClass<WithClause>(clauses.at(4));
}

TEST_CASE("Test General Ranking") {
  string query = "print s1, s2; Select BOOLEAN "
                 "such that Follows(s1, s2) and Affects*(s1, s2) and Next*(s1, s2) and Next(s1, s2) "
                 "with s1.varName = \"x\"";
  SourceParserStub exprParser;
  QueryParser parser(&exprParser);
  QueryPlanner planner;

  PQLQueryPtr pqlQuery = parser.parseQuery(&query);
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();

  QueryPlanPtr plan = planner.getExecutionPlan(pqlQuery.get(), overrideTable.get());
  REQUIRE(!plan->isEmpty());
  REQUIRE(plan->getGroupCount() == 1);

  auto group = plan->getGroup(0);
  REQUIRE(!group->canBeEmpty());

  auto clauses = group->getConditionalClauses();
  assertIsClass<WithSelectClause>(clauses.at(0));
  assertIsEitherClass<FollowsClause, NextClause>(clauses.at(1));
  assertIsEitherClass<FollowsClause, NextClause>(clauses.at(2));
  assertIsClass<NextTClause>(clauses.at(3));
  assertIsClass<AffectsTClause>(clauses.at(4));
}
