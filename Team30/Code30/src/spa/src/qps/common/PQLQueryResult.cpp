#include "PQLQueryResult.h"

using std::move, std::make_unique;

PQLQueryResult::PQLQueryResult():
    isStaticFalse(false),
    isStaticResult(false),
    error("") {}

bool PQLQueryResult::isEmpty() {
  return combinedTable.size() == 0;
}

bool PQLQueryResult::isStatic() {
  return isStaticResult;
}

void PQLQueryResult::setIsStaticFalse(bool staticRes) {
  isStaticResult = true;
  isStaticFalse = staticRes;
}

bool PQLQueryResult::isFalse() {
  return isStaticResult && isStaticFalse;
}

string PQLQueryResult::getError() {
  return error;
}

void PQLQueryResult::setError(string errorMessage) {
  error = errorMessage;
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
  resultIndex[name] = colMap.size();
  colMap.push_back(make_unique<ColMapItem>());
}

QueryResultTableRow *PQLQueryResult::getTableRowAt(int rowIndex) {
  return &combinedTable.at(rowIndex);
}

void PQLQueryResult::putTableRow(vector<QueryResultItemPtr> row) {
  int newRowNum = combinedTable.size();
  for (int i = 0; i < colMap.size(); i++) {
    ColMapItem* map = colMap.at(i).get();
    map->emplace(*row.at(i), newRowNum);
  }

  combinedTable.push_back(move(row));
}

int PQLQueryResult::getRowCount() {
  return combinedTable.size();
}

RowSet *PQLQueryResult::getRowsWithValue(PQLSynonymName name,
                                         QueryResultItem* value) {
  return getRowsWithValue(getSynonymCol(name), value);
}

RowSet *PQLQueryResult::getRowsWithValue(ResultTableCol column,
                                         QueryResultItem* value) {
  ColMapItem* item = colMap.at(column).get();
  if (item->find(*value) == item->end()) {
    return nullptr;
  }
  return &item->at(*value);
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

      bool isMatch = true;
      for (auto it : pqr.resultIndex) {
        int otherIndex = it.second;
        int thisIndex = resultIndex.at(it.first);

        if (*combinedTable[j][thisIndex].get() != *pqr.combinedTable[i][otherIndex].get()) {
          isMatch = false;
          break;
        }
      }

      if (isMatch) {
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
