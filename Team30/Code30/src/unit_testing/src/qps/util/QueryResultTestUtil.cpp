#include <memory>
#include <vector>
#include <utility>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/resulttable/ResultGroupFactory.h"
#include "qps/common/resulttable/SynonymResultTable.h"

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
      result->putTableRow(std::move(toInsert.at(i)));
    }

    return result;
  }

  static ResultGroupPtr buildResultGroup(const ExpectedParams &expectedParams, AttributedSynonymList* syns) {
    auto names = make_unique<vector<PQLSynonymName>>();
    for (auto it : *syns) {
      names->push_back(it.getName());
    }

    auto queryResult = buildExpected(expectedParams);
    return ResultGroupFactory::extractResults(queryResult.get(), names.get());
  }

  static unique_ptr<SynonymResultTable> buildExpectedTable(const ExpectedParams &expectedParams, AttributedSynonymList* syns) {
    auto result = make_unique<SynonymResultTable>(syns->empty(), true);
    result->addResultGroup(std::move(buildResultGroup(expectedParams, syns)));
    return result;
  }

  static unique_ptr<SynonymResultTable> buildExpectedTable(vector<ExpectedParams> expectedParams,
                                                           vector<AttributedSynonymList*> synList) {
    auto result = make_unique<SynonymResultTable>(false, true);
    for (size_t i = 0; i < expectedParams.size(); i++) {
      AttributedSynonymList* syns = synList[i];
      result->addResultGroup(std::move(buildResultGroup(expectedParams[i], syns)));
    }
    return result;
  }
};
