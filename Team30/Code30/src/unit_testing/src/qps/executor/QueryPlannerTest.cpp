#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/clauses/FollowsClause.h"
#include "qps/executor/QueryPlanner.h"
#include "qps/common/PQLQuerySynonym.h"

using std::make_unique, std::unordered_map, std::unique_ptr;

PQLQuerySynonym PQL_RESULT_VAR{PQL_SYN_TYPE_STMT, "a"};
PQLQuerySynonym PQL_RESULT_VAR2{PQL_SYN_TYPE_STMT, "b"};
unordered_map<string, PQLQuerySynonym> PQL_VAR_MAP({{"a", PQL_RESULT_VAR}});

QueryPlanner queryPlanner;

// Will not have select clause
TEST_CASE("Plan where a clause is using target declaration variable") {
  unique_ptr<FollowsClause> c(new FollowsClause(ClauseArgument(PQL_RESULT_VAR), ClauseArgument(5)));
  vector<shared_ptr<Clause>> clauses;
  clauses.push_back(move(c));

  PQLQuery* query = new PQLQuery(PQL_VAR_MAP, PQL_RESULT_VAR, clauses);
  shared_ptr<QueryPlan> queryPlan = queryPlanner.getExecutionPlan(query);
  REQUIRE(queryPlan->hasSelectClause() == false);
  REQUIRE(queryPlan->getConditionalClauses().size() == clauses.size());
}

// Will have select clause
TEST_CASE("Plan where a clause is not using target declaration variable") {
  unique_ptr<FollowsClause> c(new FollowsClause(ClauseArgument(PQL_RESULT_VAR2), ClauseArgument(5)));
  vector<shared_ptr<Clause>> clauses;
  clauses.push_back(move(c));

  PQLQuery* query = new PQLQuery(PQL_VAR_MAP, PQL_RESULT_VAR, clauses);
  shared_ptr<QueryPlan> queryPlan = queryPlanner.getExecutionPlan(query);
  REQUIRE(queryPlan->hasSelectClause());
  REQUIRE(queryPlan->getConditionalClauses().size() == clauses.size());
}

TEST_CASE("Plan where query is only Select") {
  PQLQuery* query = new PQLQuery(PQL_VAR_MAP, PQL_RESULT_VAR, vector<shared_ptr<Clause>>());
  shared_ptr<QueryPlan> queryPlan = queryPlanner.getExecutionPlan(query);

  REQUIRE(queryPlan->hasSelectClause());
  REQUIRE(queryPlan->getConditionalClauses().size() == 0);
}