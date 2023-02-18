#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/clauses/SelectClause.h"
#include "qps/executor/QueryOrchestrator.h"
#include "OrchestratorPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"

using std::shared_ptr, std::unordered_map, std::unordered_set, std::make_unique;
// TODO(KwanHW): Include multiple clauses test

vector<shared_ptr<IEvaluatable>> EMPTY_CONDITIONALS;
unordered_set<int> EXPECTED_STATEMENT_LINES({1, 2, 3});
unordered_set<string> EXPECTED_ENTITIES({"x", "y", "z"});

TEST_CASE("Queries with Select only") {
  // Set-up PKB Stub
  PKB pkbStore;
  PkbQueryHandler* pkb = new OrchestratorPKBStub(&pkbStore);
  auto handler = shared_ptr<PkbQueryHandler>(pkb);
  QueryLauncher launcher(handler);
  QueryOrchestrator orchestrator(launcher);

  unique_ptr<PQLQuery> query;
  PQLQuerySynonym targetVariable;
  vector<shared_ptr<Clause>> emptyClause;
  PQLQueryResultPtr expectedResult;
  PQLQueryResultPtr actualResult;

  // Statement
  // TODO(KwanHW): To implement once calls are supported
  vector<PQLSynonymType> stmtTypes{
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_ASSIGN
  };

  for (auto stmtType : stmtTypes) {
    expectedResult = TestQueryResultBuilder::buildExpected(ExpectedParams{
        {"s", QueryResultItemVector{
            QueryResultItem(1),
            QueryResultItem(2),
            QueryResultItem(3)
        }}
    });
    targetVariable = {stmtType, "s"};
    auto selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
    auto queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
    query = make_unique<PQLQuery>(
        unordered_map<string, PQLQuerySynonym>({{"s", targetVariable}}),
        targetVariable,
        emptyClause);
    actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
    REQUIRE(*expectedResult == *actualResult);
  }

  // TODO(KwanHW): To implement once procedures are supported
  vector<PQLSynonymType> entTypes{
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_VARIABLE
  };

  for (auto entType : entTypes) {
    expectedResult = TestQueryResultBuilder::buildExpected(ExpectedParams{
        {"ent", QueryResultItemVector{
            QueryResultItem("x"),
            QueryResultItem("y"),
            QueryResultItem("z")
        }}
    });
    targetVariable = {entType, "ent"};
    auto selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
    auto queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
    query = make_unique<PQLQuery>(
        unordered_map<string, PQLQuerySynonym>({{"ent", targetVariable}}),
        targetVariable,
        emptyClause);
    actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
    REQUIRE(*expectedResult == *actualResult);
  }
}

TEST_CASE("Queries that uses target declaration variable (no Select Clause)") {

}

TEST_CASE("Queries that do not use target declaration variable (have Select Clause)") {

}
