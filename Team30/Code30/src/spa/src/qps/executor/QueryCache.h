#pragma once

#include <memory>
#include "qps/common/CacheTable.h"

class QueryCache {
 private:
  CacheTable affectsCache;

 public:
  CacheTable *getAffectsCache();
};

typedef unique_ptr<QueryCache> QueryCachePtr;
