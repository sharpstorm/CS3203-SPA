#pragma once

#include <utility>

#include "qps/common/CacheTable.h"

using std::unordered_set;

class CacheAgent {
  CacheTable nextTCache;
  CacheTable affectsCache;
  CacheTable affectsTCache;

 public:
  void addToNextTCache(unordered_set<StmtValue> entries);
  void addToAffectsCache(unordered_set<StmtValue> entries);
  void addToAffectsTCache(unordered_set<StmtValue> entries);
};