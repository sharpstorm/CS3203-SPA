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
  auto pkb = make_unique<OrchestratorPKBStub>(&pkbStore);
  QueryLauncher launcher(pkb.get());
  QueryOrchestrator orchestrator(launcher);

  unique_ptr<PQLQuery> query;
  PQLQuerySynonym targetVariable;
  vector<shared_ptr<Clause>> emptyClause;
  unique_ptr<SynonymResultTable> expectedResult;
  unique_ptr<SynonymResultTable> actualResult;

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
    auto querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym{stmtType, "s"});
    auto synPtr = querySyn.get();
    PQLQuerySynonymProxy proxy(&synPtr);
    AttributedSynonym attrSyn = AttributedSynonym(proxy);
    auto synList = make_unique<AttributedSynonymList>(AttributedSynonymList({attrSyn}));
    expectedResult = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
        {"s", QueryResultItemVector{
            QueryResultItem(1),
            QueryResultItem(2),
            QueryResultItem(3)
        }}
    }, synList.get());

    auto selectClause = shared_ptr<IEvaluatable>(new SelectClause(proxy));
    auto group = make_unique<QueryGroupPlan>(vector<IEvaluatableSPtr>{selectClause},
                                             vector<PQLSynonymName>{"s"});
    vector<QueryGroupPlanPtr> groups;
    groups.push_back(std::move(group));
    auto queryPlan = make_unique<QueryPlan>(std::move(groups));
    auto targetSyns = make_unique<AttributedSynonymList>(AttributedSynonymList ({attrSyn}));
    OverrideTablePtr overrideTable = make_unique<OverrideTable>();
    actualResult = unique_ptr<SynonymResultTable>(orchestrator.execute(queryPlan.get(), overrideTable.get()));
    REQUIRE(*expectedResult == *actualResult);
  }

//  // TODO(KwanHW): To implement once procedures are supported
  vector<PQLSynonymType> entTypes{
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_VARIABLE
  };

  for (auto entType : entTypes) {
    auto querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym{entType, "ent"});
    auto synPtr = querySyn.get();
    PQLQuerySynonymProxy proxy(&synPtr);

    AttributedSynonym attrSyn = AttributedSynonym(proxy);
    auto synList = make_unique<AttributedSynonymList>(AttributedSynonymList({attrSyn}));
    expectedResult = TestQueryResultBuilder::buildExpectedTable(ExpectedParams{
        {"ent", QueryResultItemVector{
            QueryResultItem("x"),
            QueryResultItem("y"),
            QueryResultItem("z")
        }}
    }, synList.get());
    auto selectClause = shared_ptr<SelectClause>(new SelectClause(proxy));
    auto group = make_unique<QueryGroupPlan>(vector<IEvaluatableSPtr>{selectClause},
                                             vector<PQLSynonymName>{"ent"});
    vector<QueryGroupPlanPtr> groups;
    groups.push_back(std::move(group));
    auto queryPlan = make_unique<QueryPlan>(std::move(groups));
    auto targetSyns = make_unique<AttributedSynonymList>(AttributedSynonymList ({attrSyn}));
    OverrideTablePtr overrideTable = make_unique<OverrideTable>();
    actualResult = unique_ptr<SynonymResultTable>(orchestrator.execute(queryPlan.get(), overrideTable.get()));
    REQUIRE(*expectedResult == *actualResult);
  }
}

TEST_CASE("Queries that uses target declaration variable (no Select Clause)") {

}

TEST_CASE("Queries that do not use target declaration variable (have Select Clause)") {

}
