#include <utility>

#include "CacheAgent.h"

using std::unordered_set;

void CacheAgent::addToNextTCache(QueryResult<StmtValue, StmtValue>* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it: entries->pairVals) {
    nextTCache.addToFullTableAt(it.first, it.second);
  }
}

void CacheAgent::addToAffectsCache(QueryResult<StmtValue, StmtValue>* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it: entries->pairVals) {
    affectsCache.addToFullTableAt(it.first, it.second);
  }
}

void CacheAgent::addToAffectsTCache(QueryResult<StmtValue, StmtValue>* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it: entries->pairVals) {
    affectsTCache.addToFullTableAt(it.first, it.second);
  }
}
