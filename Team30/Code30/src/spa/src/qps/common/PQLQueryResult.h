#pragma once

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "QueryResultItem.h"

using std::pair, std::string, std::unordered_map, std::unordered_set,
    std::vector, std::move, std::make_unique, std::unique_ptr;

typedef vector<QueryResultItemPtr> QueryResultTableRow;
typedef vector<QueryResultTableRow> QueryResultTable;
typedef int ResultTableCol;
typedef unordered_set<int> RowSet;

typedef QueryResultItemMap<RowSet> ColMapItem;
typedef unique_ptr<ColMapItem> ColMapItemPtr;

class PQLQueryResult {
 private:
  unordered_map<PQLSynonymName, ResultTableCol> resultIndex;
  QueryResultTable combinedTable;
  vector<ColMapItemPtr> colMap;

  string error;
  bool isStaticResult;
  bool isStaticFalse;

 public:
  PQLQueryResult();
  virtual ~PQLQueryResult() = default;

  template<class T, class U>
  void add(const PQLSynonymName &leftName,
           const PQLSynonymName &rightName,
           const pair_set<T, U> &data);

  template<class T>
  void add(const PQLSynonymName &name,
           const unordered_set<T> &data);

  bool isEmpty();
  bool isFalse();
  void setIsStaticFalse(bool staticRes);
  bool isStatic();

  string getError();
  void setError(string errorMessage);

  unordered_map<PQLSynonymName, ResultTableCol>* getSynonyms();
  ResultTableCol getSynonymCol(const PQLSynonymName &name);
  void putSynonym(const PQLSynonymName &name);

  QueryResultTableRow* getTableRowAt(int rowIndex);
  void putTableRow(vector<QueryResultItemPtr> row);
  int getRowCount();
  RowSet* getRowsWithValue(ResultTableCol column, QueryResultItem* value);

  bool operator ==(const PQLQueryResult &pqr) const;

  static const int NO_COL = -1;
};

typedef unique_ptr<PQLQueryResult> PQLQueryResultPtr;

template<class T>
void PQLQueryResult::add(
    const PQLSynonymName &name,
    const unordered_set<T> &data) {
  putSynonym(name);
  ColMapItem* map = colMap.at(colMap.size() - 1).get();

  for (T item : data) {
    QueryResultTableRow row{};
    row.push_back(make_unique<QueryResultItem>(item));
    putTableRow(std::move(row));
  }
}

template<class T, class U>
void PQLQueryResult::add(const PQLSynonymName &leftName,
                         const PQLSynonymName &rightName,
                         const pair_set<T, U> &data) {
  putSynonym(leftName);
  putSynonym(rightName);

  for (pair<T, U> pair : data) {
    QueryResultTableRow row{};
    row.push_back(make_unique<QueryResultItem>(pair.first));
    row.push_back(make_unique<QueryResultItem>(pair.second));
    putTableRow(std::move(row));
  }
}
