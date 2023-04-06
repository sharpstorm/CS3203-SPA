#include "PQLQueryResult.h"

using std::make_unique;

PQLQueryResult::PQLQueryResult() :
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
  return combinedTable.empty();
}

void PQLQueryResult::setIsStaticFalse(const bool staticRes) {
  isStaticResult = true;
  isStaticFalse = staticRes;
}

bool PQLQueryResult::isFalse() const {
  return (isStaticResult && isStaticFalse)
      || (!isStaticResult && isEmpty());
}

const SynonymColMap *PQLQueryResult::getSynonyms() const {
  return &resultIndex;
}

ResultTableCol PQLQueryResult::getSynonymCol(const PQLSynonymName &name) const {
  const auto it = resultIndex.find(name);
  if (it == resultIndex.end()) {
    return NO_COL;
  }
  return it->second;
}

void PQLQueryResult::putSynonym(const PQLSynonymName &name) {
  resultIndex[name] = colMaps.size();
  colMaps.push_back(make_unique<ColMap>());
}

const QueryResultTableRow *PQLQueryResult::getTableRowAt(
    ResultTableRow rowIndex) const {
  return &combinedTable.at(rowIndex);
}

void PQLQueryResult::putTableRow(const vector<QueryResultItem *> &row) {
  ResultTableRow newRowNum = combinedTable.size();
  for (size_t i = 0; i < colMaps.size(); i++) {
    ColMap *map = colMaps.at(i).get();
    const QueryResultItem *item = row.at(i);
    auto set = &(*map)[*item];
    set->insert(newRowNum);
  }

  combinedTable.push_back(row);
}

int PQLQueryResult::getRowCount() const {
  return combinedTable.size();
}

RowSetPtr PQLQueryResult::getRowsWithValue(const ResultTableCol column,
                                           QueryResultItem *value) const {
  ColMap *colMap = colMaps.at(column).get();
  const auto &it = colMap->find(*value);
  if (it == colMap->end()) {
    return nullptr;
  }
  return make_unique<RowSet>(it->second);
}

bool PQLQueryResult::operator==(const PQLQueryResult &pqr) const {
  if (resultIndex.size() != pqr.resultIndex.size()
      || combinedTable.size() != pqr.combinedTable.size()) {
    return false;
  }

  for (const auto &it : pqr.resultIndex) {
    if (resultIndex.find(it.first) == resultIndex.end()) {
      return false;
    }
  }

  for (size_t i = 0; i < pqr.combinedTable.size(); i++) {
    bool isFound = false;
    for (size_t j = 0; j < combinedTable.size(); j++) {
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
                              ResultTableRow myRowIndex,
                              ResultTableRow otherRowIndex) const {
  for (const auto &it : other.resultIndex) {
    int otherIndex = it.second;
    int thisIndex = resultIndex.at(it.first);

    if (*combinedTable[myRowIndex][thisIndex] !=
        *other.combinedTable[otherRowIndex][otherIndex]) {
      return false;
    }
  }
  return true;
}
