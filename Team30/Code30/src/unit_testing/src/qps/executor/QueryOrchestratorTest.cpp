#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "qps/clauses/SelectClause.h"
#include "qps/executor/QueryOrchestrator.h"
#include "OrchestratorPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/arguments/SynonymArgument.h"

using std::shared_ptr, std::unordered_map, std::unordered_set, std::make_unique;

QueryResultItemVector STMT_ROW {TestResultItem(1), TestResultItem(2), TestResultItem(3)};
QueryResultItemVector ENT_ROW { TestResultItem("x"), TestResultItem("y"), TestResultItem("z")};
ExpectedParams EXPECTED_STMTS {{"s", STMT_ROW}};
ExpectedParams EXPECTED_ENTITIES({{"ent", ENT_ROW}});

class QueryGroupPlanSpy : public QueryGroupPlan {
 public:
  QueryGroupPlanSpy(IEvaluatableRefList conditionalClauses,
                    PQLSynonymNameList selectables,
                    const ComplexityScore &score) :
      QueryGroupPlan(conditionalClauses, selectables, score) {}
  void assertOwnedSize(int size) {
    auto &owned = getOwnedEvals();
    REQUIRE(owned.size() == size);
    for (const auto &it : owned) {
      REQUIRE(it.get() != nullptr);
    }
  }
};

TEST_CASE("Queries with Select only") {
  // Set-up PKB Stub
  PKB pkbStore;
  auto pkb = make_unique<OrchestratorPKBStub>(&pkbStore);
  QueryLauncher launcher(pkb.get());

  unique_ptr<PQLQuery> query;
  vector<shared_ptr<Clause>> emptyClause;
  unique_ptr<ProjectableTable> expectedResult;
  unique_ptr<ProjectableTable> actualResult;

  // Statement
  vector<PQLSynonymType> stmtTypes{
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_ASSIGN,
      PQL_SYN_TYPE_CALL
  };

  for (auto stmtType : stmtTypes) {
    auto querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym{stmtType, "s"});
    auto synPtr = querySyn.get();
    PQLQuerySynonymProxy proxy(&synPtr);
    AttributedSynonym attrSyn = AttributedSynonym(proxy);
    auto synList = make_unique<AttributedSynonymList>(AttributedSynonymList({attrSyn}));
    expectedResult = TestQueryResultBuilder::buildExpectedTable(EXPECTED_STMTS, synList.get());

    IEvaluatablePtr selectClause = make_unique<SelectClause>(proxy);
    auto group = make_unique<QueryGroupPlan>(
        vector<IEvaluatable*>{selectClause.get()},
        vector<PQLSynonymName>{"s"}, 0);
    vector<QueryGroupPlanPtr> groups;
    groups.push_back(std::move(group));
    auto queryPlan = make_unique<QueryPlan>(std::move(groups));
    auto targetSyns = make_unique<AttributedSynonymList>(AttributedSynonymList ({attrSyn}));
    OverrideTable overrideTable;
    QueryOrchestrator orchestrator(&launcher);
    actualResult = unique_ptr<ProjectableTable>(orchestrator.execute(queryPlan.get(), &overrideTable));
    REQUIRE(*expectedResult == *actualResult);
  }

  vector<PQLSynonymType> entTypes{
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_PROCEDURE
  };

  for (auto entType : entTypes) {
    auto querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym{entType, "ent"});
    auto synPtr = querySyn.get();
    PQLQuerySynonymProxy proxy(&synPtr);

    auto querySyn2 = make_unique<PQLQuerySynonym>(PQLQuerySynonym{entType, "entOwned"});
    auto synPtr2 = querySyn2.get();
    PQLQuerySynonymProxy proxy2(&synPtr2);

    AttributedSynonym attrSyn = AttributedSynonym(proxy);
    auto synList = make_unique<AttributedSynonymList>(AttributedSynonymList({attrSyn}));
    expectedResult = TestQueryResultBuilder::buildExpectedTable(EXPECTED_ENTITIES, synList.get());
    SelectClause selectClause(proxy);
    vector<IEvaluatablePtr> dummyOwned;
    dummyOwned.push_back(make_unique<SelectClause>(proxy2));
    auto group = make_unique<QueryGroupPlanSpy>(vector<IEvaluatable*>{&selectClause},
                                             vector<PQLSynonymName>{"ent"}, 0);
    group->adoptEvals(&dummyOwned);
    REQUIRE(dummyOwned.at(0).get() == nullptr);
    group->assertOwnedSize(1);
    vector<QueryGroupPlanPtr> groups;
    groups.push_back(std::move(group));
    auto queryPlan = make_unique<QueryPlan>(std::move(groups));
    auto targetSyns = make_unique<AttributedSynonymList>(AttributedSynonymList ({attrSyn}));
    OverrideTablePtr overrideTable = make_unique<OverrideTable>();
    QueryOrchestrator orchestrator(&launcher);
    actualResult = unique_ptr<ProjectableTable>(orchestrator.execute(queryPlan.get(), overrideTable.get()));
    REQUIRE(*expectedResult == *actualResult);
  }
}

TEST_CASE("Queries that uses target declaration variable (no Select Clause)") {

}

TEST_CASE("Queries that do not use target declaration variable (have Select Clause)") {

}
