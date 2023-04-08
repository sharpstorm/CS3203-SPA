#pragma once

#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "QueryResultItem.h"

using std::vector, std::map;

typedef map<QueryResultItem *, QueryResultItem *> QueryResultItemMapping;

class OrphanedResultItemPool {
 private:
  vector<QueryResultItemPtr> owned;
  QueryResultItemMapping lookupMap;

 public:
  explicit OrphanedResultItemPool() = default;
  void adopt(QueryResultItemPtr item);
  QueryResultItem *getMappingFor(QueryResultItem *item);
  void insertMapping(QueryResultItem *from, QueryResultItem *to);
};

typedef unique_ptr<OrphanedResultItemPool> OrphanedResultItemPoolPtr;
