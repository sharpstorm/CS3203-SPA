#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/executor/planner/QueryPlanner.h"
#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/clauses/such_that/FollowsClauses.h"

using std::make_unique, std::unordered_map, std::unique_ptr;

PQLQuerySynonym PQL_RESULT_VAR_RAW{PQL_SYN_TYPE_STMT, "a"};
PQLQuerySynonym PQL_RESULT_VAR2_RAW{PQL_SYN_TYPE_STMT, "b"};
PQLQuerySynonym* PQL_RESULT_VAR_PTR = &PQL_RESULT_VAR_RAW;
PQLQuerySynonym* PQL_RESULT_VAR2_PTR = &PQL_RESULT_VAR2_RAW;
PQLQuerySynonymProxy PQL_RESULT_VAR(&PQL_RESULT_VAR_PTR);
PQLQuerySynonymProxy PQL_RESULT_VAR2(&PQL_RESULT_VAR2_PTR);

AttributedSynonym ATTR_RESULT_VAR(PQL_RESULT_VAR);
AttributedSynonym ATTR_RESULT_VAR2(PQL_RESULT_VAR2);
AttributedSynonymList PQL_RESULT_VARS{ATTR_RESULT_VAR};
unordered_map<string, PQLQuerySynonym> PQL_VAR_MAP({{"a", PQL_RESULT_VAR_RAW}});

// Will not have select clause
TEST_CASE("Plan where a clause is using target declaration variable") {
  auto c = make_unique<FollowsClause>(
      ClauseArgumentFactory::create(PQL_RESULT_VAR),
      ClauseArgumentFactory::create(5));
  vector<ClausePtr> clauses;
  clauses.push_back(std::move(c));

  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr),
                                     PQL_RESULT_VARS,
                                     std::move(clauses),
                                     vector<ConstraintPtr>());
  auto overrides = make_unique<OverrideTable>();
  QueryPlanPtr queryPlan = QueryPlanner().getExecutionPlan(query.get(), overrides.get());
  REQUIRE(queryPlan->getGroupCount() == 1);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == 1);
}

// Will have select clause
TEST_CASE("Plan where a clause is not using target declaration variable") {
  auto c = make_unique<FollowsClause>(
      ClauseArgumentFactory::create(PQL_RESULT_VAR2),
      ClauseArgumentFactory::create(5));
  vector<ClausePtr> clauses;
  clauses.push_back(std::move(c));

  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr),
                                     PQL_RESULT_VARS,
                                     std::move(clauses),
                                     vector<ConstraintPtr>());
  auto overrides = make_unique<OverrideTable>();
  QueryPlanPtr queryPlan = QueryPlanner().getExecutionPlan(query.get(), overrides.get());
  REQUIRE(queryPlan->getGroupCount() == 2);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == 1);
  REQUIRE(queryPlan.get()->getGroup(1)->getConditionalClauses().size() == 1);
}

TEST_CASE("Plan where query is only Select") {
  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr), PQL_RESULT_VARS,
                                     vector<ClausePtr>(), vector<ConstraintPtr>());
  auto overrides = make_unique<OverrideTable>();
  QueryPlanPtr queryPlan = QueryPlanner().getExecutionPlan(query.get(), overrides.get());

  REQUIRE(queryPlan->getGroupCount() == 1);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == 1);
}
