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
  return &synIndex;
}

ResultTableCol PQLQueryResult::getSynonymCol(const PQLSynonymName &name) const {
  const auto &it = synIndex.find(name);
  if (it == synIndex.end()) {
    return NO_COL;
  }
  return it->second;
}

void PQLQueryResult::putSynonym(const PQLSynonymName &name) {
  synIndex[name] = colValueMaps.size();
  colValueMaps.push_back(make_unique<ColValueMap>());
}

const QueryResultTableRow *PQLQueryResult::getTableRowAt(
    ResultTableRow rowIndex) const {
  return &combinedTable.at(rowIndex);
}

void PQLQueryResult::putTableRow(const vector<QueryResultItem *> &row) {
  ResultTableRow newRowNum = combinedTable.size();
  for (size_t i = 0; i < colValueMaps.size(); i++) {
    ColValueMap *map = colValueMaps.at(i).get();
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
  ColValueMap *colMap = colValueMaps.at(column).get();
  const auto &it = colMap->find(*value);
  if (it == colMap->end()) {
    return nullptr;
  }
  return make_unique<RowSet>(it->second);
}

bool PQLQueryResult::operator==(const PQLQueryResult &pqr) const {
  if (synIndex.size() != pqr.synIndex.size()
      || combinedTable.size() != pqr.combinedTable.size()) {
    return false;
  }

  for (const auto &it : pqr.synIndex) {
    if (synIndex.find(it.first) == synIndex.end()) {
      return false;
    }
  }

  for (size_t i = 0; i < combinedTable.size(); i++) {
    if (!hasRowIn(i, pqr)) {
      return false;
    }
  }

  return true;
}

bool PQLQueryResult::hasRowIn(const ResultTableRow &target,
                              const PQLQueryResult &haystack) const {
  for (size_t i = 0; i < haystack.combinedTable.size(); i++) {
    if (haystack.combinedTable[i].size() != combinedTable[target].size()) {
      return false;
    }

    if (matchRow(haystack, target, i)) {
      return true;
    }
  }
  return false;
}

bool PQLQueryResult::matchRow(const PQLQueryResult &other,
                              const ResultTableRow &rowIndex,
                              const ResultTableRow otherRowIndex) const {
  for (const auto &it : other.synIndex) {
    int otherIndex = it.second;
    int thisIndex = synIndex.at(it.first);

    if (*combinedTable[rowIndex][thisIndex] !=
        *other.combinedTable[otherRowIndex][otherIndex]) {
      return false;
    }
  }
  return true;
}
