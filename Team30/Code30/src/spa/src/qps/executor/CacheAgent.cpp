#include <utility>

#include "CacheAgent.h"

using std::unordered_set;

void CacheAgent::addToNextTCache(StmtStmtQueryResult* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it : entries->pairVals) {
    nextTCache.addToFullTableAt(it.first, it.second);
  }
}

void CacheAgent::addToAffectsCache(StmtStmtQueryResult* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it : entries->pairVals) {
    affectsCache.addToFullTableAt(it.first, it.second);
  }
}

void CacheAgent::addToAffectsTCache(StmtStmtQueryResult* entries) {
  // TODO(KwanHW): Write logic to determine insert to partial or full
  for (auto it : entries->pairVals) {
    affectsTCache.addToFullTableAt(it.first, it.second);
  }
}
