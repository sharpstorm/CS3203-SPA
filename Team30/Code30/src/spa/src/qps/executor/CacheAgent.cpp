#include <memory>

#include "CacheAgent.h"

using std::make_unique;

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

StmtStmtQueryResultPtr CacheAgent::queryNextTCache(const StmtRef& left,
                                                   const StmtRef& right) {
  StmtValue leftVal = left.getValue();
  StmtValue rightVal = right.getValue();

  CacheRow* result = nextTCache.queryFull(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  result = nextTCache.queryPartial(leftVal, rightVal);

  return toQueryResult(left, right, result);
}

StmtStmtQueryResultPtr CacheAgent::queryAffectsCache(const StmtRef& left,
                                                     const StmtRef& right) {
  StmtValue leftVal = left.getValue();
  StmtValue rightVal = right.getValue();

  CacheRow* result = affectsCache.queryFull(leftVal, rightVal);

  if (result != nullptr && !result->empty()) {
    return toQueryResult(left, right, result);
  }

  result = affectsCache.queryPartial(leftVal, rightVal);

  return toQueryResult(left, right, result);
}

StmtStmtQueryResultPtr CacheAgent::toQueryResult(const StmtRef& left,
                                                 const StmtRef& right,
                                                 CacheRow *row) {
  StmtStmtQueryResultPtr result = make_unique<StmtStmtQueryResult>();

  if (row == nullptr || row->empty()) {
    return result;
  }

  // (static, static)
  if (left.isKnown() && right.isKnown()) {
    result->add(left.getValue(), right.getValue());
    return result;
  }

  // (syn, static)
  if (!left.isKnown()) {
    for (auto r : *row) {
      result->add(r, right.getValue());
    }
  }

  // (static, syn)
  if (!right.isKnown()) {
    for (auto r : *row) {
      result->add(left.getValue(), r);
    }
  }

  return result;
}
bool CacheAgent::entryExistsNextT(StmtRef left, StmtRef right) {
  return false;
}
