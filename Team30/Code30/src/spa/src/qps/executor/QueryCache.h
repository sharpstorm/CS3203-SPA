#pragma once

#include "qps/common/CacheTable.h"
class QueryCache {
 private:
  CacheTable nextTCache;
  CacheTable affectsCache;

 public:
  CacheTable *getNextTCache();
  CacheTable *getAffectsCache();
};

typedef unique_ptr<QueryCache> QueryCachePtr;