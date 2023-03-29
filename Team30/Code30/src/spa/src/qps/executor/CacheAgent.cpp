#include <utility>

#include "CacheAgent.h"

using std::unordered_set;

void CacheAgent::addToNextTCache(StmtStmtQueryResult* entries) {
  for (auto it : entries->pairVals) {
    nextTCache.addEntry(it.first, it.second);
  }
}

void CacheAgent::addToAffectsCache(StmtStmtQueryResult* entries) {
  for (auto it : entries->pairVals) {
    affectsCache.addEntry(it.first, it.second);
  }
}

void CacheAgent::addToAffectsTCache(StmtStmtQueryResult* entries) {
  for (auto it : entries->pairVals) {
    affectsTCache.addEntry(it.first, it.second);
  }
}

StmtStmtQueryResult CacheAgent::queryNextTCache(StmtRef left, StmtRef right) {
  StmtValue leftVal = left.getValue();
  StmtValue rightVal = right.getValue();

  CacheRow* result = nextTCache.queryFull(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  result = nextTCache.queryPartial(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  return {};
}

StmtStmtQueryResult CacheAgent::queryAffectsCache(StmtRef left, StmtRef right) {
  return {};
}

StmtStmtQueryResult CacheAgent::queryAffectsTCache(StmtRef left,
                                                   StmtRef right) {
  return {};
}

StmtStmtQueryResult CacheAgent::toQueryResult(StmtRef left,
                                              StmtRef right,
                                              CacheRow *row) {
  StmtStmtQueryResult result;

  if (row->empty()) {
    return result;
  }

  if (left.isKnown() && right.isKnown()) {
    result.add(left.getValue(), right.getValue());
    return result;
  }

  if (!left.isKnown()) {
    for (auto r : *row) {
      result.add(r, right.getValue());
    }
  }

  if (!right.isKnown()) {
    for (auto r : *row) {
      result.add(left.getValue(), r);
    }
  }

  return result;
}
