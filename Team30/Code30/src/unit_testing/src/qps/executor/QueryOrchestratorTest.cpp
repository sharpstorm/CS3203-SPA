#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/clauses/SelectClause.h"
#include "qps/executor/QueryOrchestrator.h"
#include "OrchestratorPKBStub.cpp"
#include "qps/common/adapters/StatementResultBuilder.h"
#include "qps/common/adapters/EntityResultBuilder.h"

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
  unique_ptr<PQLQueryResult> expectedResult;
  unique_ptr<PQLQueryResult> actualResult;

  // StatementTypes
  // Statement
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("s", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_STMT, "s"};
  auto selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  auto queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"s", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // Read
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("re", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_READ, "re"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"re", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // Print
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("pn", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_PRINT, "pn"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"pn", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // TODO(KwanHW): To implement once calls are supported
  // Call

  // While
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("w", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_WHILE, "w"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"w", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // If
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("ifs", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_IF, "ifs"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"ifs", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // Assign
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToStatementMap("a", StatementResultBuilder::buildStatementResult(EXPECTED_STATEMENT_LINES));
  targetVariable = {PQL_SYN_TYPE_ASSIGN, "a"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"a", targetVariable}}),
      targetVariable,
      emptyClause);
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // EntityTypes
  // Constant
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToEntityMap("c", EntityResultBuilder::buildEntityResult(EXPECTED_ENTITIES));
  targetVariable = {PQL_SYN_TYPE_CONSTANT, "c"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"c", targetVariable}}),
      targetVariable,
      emptyClause
      );
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);

  // TODO(KwanHW): To implement once procedures are supported
  // Procedure

  // Variable
  expectedResult = make_unique<PQLQueryResult>();
  expectedResult->addToEntityMap("v", EntityResultBuilder::buildEntityResult(EXPECTED_ENTITIES));
  targetVariable = {PQL_SYN_TYPE_VARIABLE, "v"};
  selectClause = shared_ptr<SelectClause>(new SelectClause(targetVariable));
  queryPlan = make_unique<QueryPlan>(selectClause, EMPTY_CONDITIONALS);
  query = make_unique<PQLQuery>(
      unordered_map<string, PQLQuerySynonym>({{"v", targetVariable}}),
      targetVariable,
      emptyClause
  );
  actualResult = unique_ptr<PQLQueryResult>(orchestrator.execute(query.get(), queryPlan.get()));
  REQUIRE(*expectedResult == *actualResult);
}

TEST_CASE("Queries that uses target declaration variable (no Select Clause)") {

}

TEST_CASE("Queries that do not use target declaration variable (have Select Clause)") {

}
