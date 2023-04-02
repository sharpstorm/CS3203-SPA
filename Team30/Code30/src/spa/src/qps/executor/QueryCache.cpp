#include "QueryCache.h"

CacheTable *QueryCache::getAffectsCache() {
  return &affectsCache;
}
