#include "CacheTable.h"

void CacheTable::addToFullTableAt(int idx, int value) {
  fullTable.at(idx).push_back(value);
}

void CacheTable::addToPartialTableAt(int idx, int value) {
  partialTable.at(idx).push_back(value);
}
bool CacheTable::inFullTable(int idx, int value) {
  if (idx > fullTable.size()) {
    return false;
  }

  vector<StmtValue> values = fullTable.at(idx);
  return valueInTable(values, value);
}

bool CacheTable::inPartialTable(int idx, int value) {
  if (idx > partialTable.size()) {
    return false;
  }

  vector<StmtValue> values = partialTable.at(idx);
  return valueInTable(values, value);
}

bool CacheTable::valueInTable(vector<StmtValue> values, int value) {
  for (auto v : values) {
    if (v == value) {
      return true;
    }
  }

  return false;
}
