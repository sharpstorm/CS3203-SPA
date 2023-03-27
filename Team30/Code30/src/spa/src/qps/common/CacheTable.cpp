#include "CacheTable.h"

void CacheTable::addToFullTableAt(int idx, int value) {
  fullTable.at(idx).push_back(value);
}

void CacheTable::addToPartialTableAt(int idx, int value) {
  partialTable.at(idx).push_back(value);
}
