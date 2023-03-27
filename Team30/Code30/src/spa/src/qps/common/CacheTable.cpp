#include "CacheTable.h"

void CacheTable::addToFullTable(const vector<StmtValue>& tableEntry) {
  fullTable.push_back(tableEntry);
}

void CacheTable::addToPartialTable(const vector<StmtValue>& tableEntry) {
  partialTable.push_back(tableEntry);
}
