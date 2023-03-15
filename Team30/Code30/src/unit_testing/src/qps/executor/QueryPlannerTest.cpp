#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/executor/planner/QueryPlanner.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/clauses/such_that/FollowsClauses.h"

using std::make_unique, std::unordered_map, std::unique_ptr;

PQLQuerySynonym PQL_RESULT_VAR{PQL_SYN_TYPE_STMT, "a"};
PQLQuerySynonym PQL_RESULT_VAR2{PQL_SYN_TYPE_STMT, "b"};
AttributedSynonym ATTR_RESULT_VAR(&PQL_RESULT_VAR);
AttributedSynonym ATTR_RESULT_VAR2(&PQL_RESULT_VAR2);
AttributedSynonymList PQL_RESULT_VARS{ATTR_RESULT_VAR};
unordered_map<string, PQLQuerySynonym> PQL_VAR_MAP({{"a", PQL_RESULT_VAR}});

// Will not have select clause
TEST_CASE("Plan where a clause is using target declaration variable") {
  auto c = make_unique<FollowsClause>(
      ClauseArgumentFactory::create(PQL_RESULT_VAR),
      ClauseArgumentFactory::create(5));
  vector<shared_ptr<Clause>> clauses;
  clauses.push_back(move(c));

  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr), PQL_RESULT_VARS, clauses,
                                     vector<ConstraintSPtr>());
  shared_ptr<QueryPlan> queryPlan = QueryPlanner().getExecutionPlan(query.get());
  REQUIRE(queryPlan->getGroupCount() == 1);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == clauses.size());
}

// Will have select clause
TEST_CASE("Plan where a clause is not using target declaration variable") {
  auto c = make_unique<FollowsClause>(
      ClauseArgumentFactory::create(PQL_RESULT_VAR2),
      ClauseArgumentFactory::create(5));
  vector<shared_ptr<Clause>> clauses;
  clauses.push_back(move(c));

  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr), PQL_RESULT_VARS,
                                     clauses, vector<ConstraintSPtr>());
  shared_ptr<QueryPlan> queryPlan = QueryPlanner().getExecutionPlan(query.get());
  REQUIRE(queryPlan->getGroupCount() == 2);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == clauses.size());
  REQUIRE(queryPlan.get()->getGroup(1)->getConditionalClauses().size() == 1);
}

TEST_CASE("Plan where query is only Select") {
  auto varMapPtr = make_unique<VariableTable>(PQL_VAR_MAP);
  auto query = make_unique<PQLQuery>(std::move(varMapPtr), PQL_RESULT_VARS,
                                     vector<ClauseSPtr>(), vector<ConstraintSPtr>());
  shared_ptr<QueryPlan> queryPlan = QueryPlanner().getExecutionPlan(query.get());

  REQUIRE(queryPlan->getGroupCount() == 1);
  REQUIRE(queryPlan.get()->getGroup(0)->getConditionalClauses().size() == 1);
}
