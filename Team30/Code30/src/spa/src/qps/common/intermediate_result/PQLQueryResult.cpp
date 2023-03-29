#include "PQLQueryResult.h"

using std::make_unique;

PQLQueryResult::PQLQueryResult():
    isStaticFalse(false),
    isStaticResult(false) {}

OrphanedResultItemPoolPtr PQLQueryResult::adoptOwnedItems(
    PQLQueryResult *other) {
  return ownedItemPool.adoptPool(&other->ownedItemPool);
}

OrphanedResultItemPoolPtr PQLQueryResult::releaseOwnedItemsTo(
    QueryResultItemPool *other) {
  return other->adoptPool(&ownedItemPool);
}

bool PQLQueryResult::isEmpty() const {
  return combinedTable.size() == 0;
}

void PQLQueryResult::setIsStaticFalse(bool staticRes) {
  isStaticResult = true;
  isStaticFalse = staticRes;
}

bool PQLQueryResult::isFalse() {
  return (isStaticResult && isStaticFalse)
      || (!isStaticResult && isEmpty());
}

unordered_map<PQLSynonymName, ResultTableCol> *PQLQueryResult::getSynonyms() {
  return &resultIndex;
}

ResultTableCol PQLQueryResult::getSynonymCol(const PQLSynonymName &name) {
  if (resultIndex.find(name) == resultIndex.end()) {
    return NO_COL;
  }
  return resultIndex[name];
}

void PQLQueryResult::putSynonym(const PQLSynonymName &name) {
  resultIndex[name] = colMaps.size();
  colMaps.push_back(make_unique<ColMap>());
}

QueryResultTableRow *PQLQueryResult::getTableRowAt(int rowIndex) {
  return &combinedTable.at(rowIndex);
}

void PQLQueryResult::putTableRow(const vector<QueryResultItem*> &row) {
  ResultTableRow newRowNum = combinedTable.size();
  for (int i = 0; i < colMaps.size(); i++) {
    ColMap* map = colMaps.at(i).get();
    const QueryResultItem *item = row.at(i);
    (*map)[*item].insert(newRowNum);
  }

  combinedTable.push_back(row);
}

int PQLQueryResult::getRowCount() {
  return combinedTable.size();
}

RowSetPtr PQLQueryResult::getRowsWithValue(ResultTableCol column,
                                           QueryResultItem* value) {
  ColMap* colMap = colMaps.at(column).get();
  if (colMap->find(*value) == colMap->end()) {
    return nullptr;
  }
  return make_unique<RowSet>(colMap->at(*value));
}

bool PQLQueryResult::operator==(const PQLQueryResult &pqr) const {
  if (resultIndex.size() != pqr.resultIndex.size()
      || combinedTable.size() != pqr.combinedTable.size()) {
    return false;
  }

  for (auto it : pqr.resultIndex) {
    if (resultIndex.find(it.first) == resultIndex.end()) {
      return false;
    }
  }

  for (int i = 0; i < pqr.combinedTable.size(); i++) {
    bool isFound = false;
    for (int j = 0; j < combinedTable.size(); j++) {
      if (combinedTable[j].size() != pqr.combinedTable[i].size()) {
        return false;
      }

      if (matchRow(pqr, j, i)) {
        isFound = true;
        break;
      }
    }
    if (!isFound) {
      return false;
    }
  }

  return true;
}

bool PQLQueryResult::matchRow(const PQLQueryResult &other,
                              const int &myRowIndex,
                              const int &otherRowIndex) const {
  for (auto it : other.resultIndex) {
    int otherIndex = it.second;
    int thisIndex = resultIndex.at(it.first);

    if (*combinedTable[myRowIndex][thisIndex] !=
        *other.combinedTable[otherRowIndex][otherIndex]) {
      return false;
    }
  }
  return true;
}
