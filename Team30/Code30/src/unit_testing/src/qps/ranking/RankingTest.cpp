#include "catch.hpp"
#include "qps/common/PQLQuery.h"
#include "qps/parser/QueryParser.h"
#include "../SourceParserStub.h"
#include "qps/executor/planner/QueryPlan.h"
#include "qps/executor/planner/QueryPlanner.h"

template <class ClauseType>
void assertIsClass(IEvaluatable* ptr) {
  auto fc = dynamic_cast<ClauseType*>(ptr);
  REQUIRE(fc != nullptr);
}

template <class ClauseType1, class ClauseType2>
void assertIsEitherClass(IEvaluatable* ptr) {
  auto c1 = dynamic_cast<ClauseType1*>(ptr);
  auto c2 = dynamic_cast<ClauseType2*>(ptr);
  REQUIRE(((c1 != nullptr) || (c2 != nullptr)));
}

TEST_CASE("Prefer With, No Constraints") {
  string query = "print s1, s2; Select BOOLEAN "
                 "such that Follows(s1, s2) and Affects*(s1, s2) and Next*(s1, s2) and Next(s1, s2) "
                 "with s1.stmt# = s2.stmt# and s1.varName = \"x\"";
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
