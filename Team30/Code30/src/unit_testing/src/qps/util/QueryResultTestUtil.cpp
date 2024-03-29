#include <memory>
#include <vector>
#include <utility>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/projector_table/ProjectableTable.h"
#include "qps/common/projector_table/ProjectableGroupFactory.h"

using std::vector, std::move, std::make_unique, std::unique_ptr;

class TestResultItem {
 private:
  StmtValue stmt;
  EntityValue ent;

 public:
  explicit TestResultItem(const StmtValue &stmt): stmt(stmt), ent(NO_ENT) {}
  explicit TestResultItem(const EntityValue &ent): stmt(NO_STMT), ent(ent) {}

  StmtValue getStmt() {
    return stmt;
  }

  EntityValue getEnt() {
    return ent;
  }

  bool isStmt() {
    return stmt != NO_STMT;
  }
};

class PQLQueryResultSpy : public PQLQueryResult {
 public:
  QueryResultItemPool* getPool() {
    return &ownedItemPool;
  }
};

typedef vector<TestResultItem> QueryResultItemVector;
typedef unordered_map<string, QueryResultItemVector> ExpectedParams;

class TestQueryResultBuilder {
 public:
  static PQLQueryResultPtr buildExpected(const ExpectedParams &expectedParams) {
    auto result = make_unique<PQLQueryResultSpy>();
    auto itemPool = result->getPool();

    vector<vector<QueryResultItem*>> toInsert;
    for (auto pair : expectedParams) {
      result->putSynonym(pair.first);

      for (int i = 0; i < pair.second.size(); i++) {
        if (toInsert.size() <= i) {
          toInsert.push_back(vector<QueryResultItem*>{});
        }
        auto value = pair.second.at(i);
        QueryResultItem* item;
        if (value.isStmt()) {
          item = itemPool->getItem(value.getStmt());
        } else {
          item = itemPool->getItem(value.getEnt());
        }
        toInsert.at(i).push_back(item);
      }
    }

    for (int i = 0; i < toInsert.size(); i++) {
      result->putTableRow(toInsert.at(i));
    }

    return result;
  }

  static unique_ptr<ProjectableTable> buildExpectedTable(const ExpectedParams &expectedParams, AttributedSynonymList* syns) {
    auto result = make_unique<ProjectableTable>(true);
    vector<PQLSynonymName>* names = new vector<PQLSynonymName>();
    for (auto it : *syns) {
      names->push_back(it.getName());
    }

    auto queryResult = buildExpected(expectedParams);
    ProjectableGroupPtr rg = ProjectableGroupFactory::extractResults(queryResult.get() , names);
    result->addResultGroup(std::move(rg));
    delete names;
    return result;
  }

  template <class ...ExpectedParams>
  static unique_ptr<ProjectableTable> buildExpectedTable(AttributedSynonymList* syns,
                                                         const ExpectedParams&... expectedParams) {
    auto result = make_unique<ProjectableTable>(true);
    ([&]
    {
      auto queryResult = buildExpected(expectedParams);
      vector<PQLSynonymName> names;
      for (auto x : expectedParams) {
        names.push_back(x.first);
      }
      ProjectableGroupPtr rg = ProjectableGroupFactory::extractResults(queryResult.get() , &names);
      result->addResultGroup(std::move(rg));
    } (), ...);
    return result;
  }

  static unique_ptr<ProjectableTable> buildExpectedTable(vector<ExpectedParams> paramsList) {
    auto result = make_unique<ProjectableTable>(true);

    for (size_t i = 0; i < paramsList.size() ;i++) {
      ExpectedParams params = paramsList[i];

      auto queryResult = buildExpected(params);
      vector<PQLSynonymName> names;
      for (auto x : params) {
        names.push_back(x.first);
      }
      ProjectableGroupPtr rg = ProjectableGroupFactory::extractResults(queryResult.get() , &names);
      result->addResultGroup(std::move(rg));
    }

    return result;
  }
};
