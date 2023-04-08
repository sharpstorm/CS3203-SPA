#pragma once

#include <memory>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "QueryResultItem.h"
#include "QueryResultItemPool.h"
#include "common/data_structs/AppendSet.h"

using std::pair, std::unordered_map, std::unordered_set,
    std::vector, std::move, std::make_unique, std::unique_ptr;

typedef int ResultTableCol;
typedef int ResultTableRow;
typedef vector<QueryResultItem *> QueryResultTableRow;
typedef vector<QueryResultTableRow> QueryResultTable;

typedef AppendSet<ResultTableRow> RowSet;

typedef QueryResultItemMap<RowSet> ColValueMap;
typedef unique_ptr<ColValueMap> ColValueMapPtr;
typedef unique_ptr<RowSet> RowSetPtr;

typedef unordered_map<PQLSynonymName, ResultTableCol> SynonymColMap;

class PQLQueryResult {
 private:
  unordered_map<PQLSynonymName, ResultTableCol> resultIndex;
  QueryResultTable combinedTable;
  vector<ColValueMapPtr> colValueMaps;

  bool isStaticResult;
  bool isStaticFalse;

  bool isEmpty() const;
  bool matchRow(const PQLQueryResult &other,
                const ResultTableRow myRowIndex,
                const ResultTableRow otherRowIndex) const;

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

  template<class T, class U>
  void addLeftConst(const PQLSynonymName &leftName,
                    const PQLSynonymName &rightName,
                    const T &leftVal,
                    const unordered_set<U> &data);

  template<class T, class U>
  void addRightConst(const PQLSynonymName &leftName,
                     const PQLSynonymName &rightName,
                     const unordered_set<T> &data,
                     const U &rightVal);

  template<class T>
  void add(const PQLSynonymName &name,
           const unordered_set<T> &data);

  bool isFalse() const;
  void setIsStaticFalse(const bool staticRes);

  const SynonymColMap *getSynonyms() const;
  ResultTableCol getSynonymCol(const PQLSynonymName &name) const;
  void putSynonym(const PQLSynonymName &name);

  const QueryResultTableRow *getTableRowAt(const ResultTableRow rowIndex) const;
  void putTableRow(const QueryResultTableRow &row);
  int getRowCount() const;
  RowSetPtr getRowsWithValue(const ResultTableCol column,
                             QueryResultItem *value) const;

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

template<class T, class U>
void PQLQueryResult::addLeftConst(const PQLSynonymName &leftName,
                                  const PQLSynonymName &rightName,
                                  const T &leftVal,
                                  const unordered_set<U> &data) {
  putSynonym(leftName);
  putSynonym(rightName);

  QueryResultItem *leftEntry = ownedItemPool.getItem(leftVal);
  for (const U &rightItem : data) {
    QueryResultItem *rightEntry = ownedItemPool.getItem(rightItem);
    QueryResultTableRow row{leftEntry, rightEntry};
    putTableRow(row);
  }
}

template<class T, class U>
void PQLQueryResult::addRightConst(const PQLSynonymName &leftName,
                                   const PQLSynonymName &rightName,
                                   const unordered_set<T> &data,
                                   const U &rightVal) {
  putSynonym(leftName);
  putSynonym(rightName);

  QueryResultItem *rightEntry = ownedItemPool.getItem(rightVal);
  for (const T &leftItem : data) {
    QueryResultItem *leftEntry = ownedItemPool.getItem(leftItem);
    QueryResultTableRow row{leftEntry, rightEntry};
    putTableRow(row);
  }
}
