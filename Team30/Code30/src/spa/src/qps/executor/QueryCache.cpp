#include "QueryCache.h"

CacheTable *QueryCache::getNextTCache() {
  return &nextTCache;
}

CacheTable *QueryCache::getAffectsCache() {
  return &affectsCache;
}
