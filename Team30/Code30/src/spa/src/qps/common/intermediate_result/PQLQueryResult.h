#pragma once

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "QueryResultItem.h"
#include "QueryResultItemPool.h"
#include "common/data_structs/AppendSet.h"

using std::pair, std::string, std::unordered_map, std::unordered_set,
    std::vector, std::move, std::make_unique, std::unique_ptr, std::set;

typedef int ResultTableCol;
typedef int ResultTableRow;
typedef vector<QueryResultItem *> QueryResultTableRow;
typedef vector<QueryResultTableRow> QueryResultTable;

typedef AppendSet<ResultTableRow> RowSet;

typedef QueryResultItemMap<RowSet> ColMap;
typedef unique_ptr<ColMap> ColMapPtr;
typedef unique_ptr<RowSet> RowSetPtr;

class PQLQueryResult {
 private:
  unordered_map<PQLSynonymName, ResultTableCol> resultIndex;
  QueryResultTable combinedTable;
  vector<ColMapPtr> colMaps;

  bool isStaticResult;
  bool isStaticFalse;

  bool isEmpty() const;
  bool matchRow(const PQLQueryResult &other,
                const int &myRowIndex,
                const int &otherRowIndex) const;

 protected:
  QueryResultItemPool ownedItemPool;

 public:
  PQLQueryResult();
  virtual ~PQLQueryResult() = default;

  OrphanedResultItemPoolPtr adoptOwnedItems(PQLQueryResult *other);
  OrphanedResultItemPoolPtr releaseOwnedItemsTo(QueryResultItemPool *other);

  template<class T, class U>
  void add(const PQLSynonymName &leftName,
           const PQLSynonymName &rightName,
           const pair_set<T, U> &data);

  template<class T>
  void add(const PQLSynonymName &name,
           const unordered_set<T> &data);

  bool isFalse();
  void setIsStaticFalse(bool staticRes);

  unordered_map<PQLSynonymName, ResultTableCol> *getSynonyms();
  ResultTableCol getSynonymCol(const PQLSynonymName &name);
  void putSynonym(const PQLSynonymName &name);

  QueryResultTableRow *getTableRowAt(int rowIndex);
  void putTableRow(const vector<QueryResultItem *> &row);
  int getRowCount();
  RowSetPtr getRowsWithValue(ResultTableCol column,
                             QueryResultItem *value);

  bool operator==(const PQLQueryResult &pqr) const;

  static const ResultTableCol NO_COL = -1;
};

typedef unique_ptr<PQLQueryResult> PQLQueryResultPtr;

template<class T>
void PQLQueryResult::add(const PQLSynonymName &name,
                         const unordered_set<T> &data) {
  putSynonym(name);
  for (const T &item : data) {
    QueryResultItem *entry = ownedItemPool.getItem(item);
    QueryResultTableRow row{entry};
    putTableRow(row);
  }
}

template<class T, class U>
void PQLQueryResult::add(const PQLSynonymName &leftName,
                         const PQLSynonymName &rightName,
                         const pair_set<T, U> &data) {
  putSynonym(leftName);
  putSynonym(rightName);

  for (const pair<T, U> &pair : data) {
    QueryResultItem *leftEntry = ownedItemPool.getItem(pair.first);
    QueryResultItem *rightEntry = ownedItemPool.getItem(pair.second);
    QueryResultTableRow row{leftEntry, rightEntry};
    putTableRow(row);
  }
}
