#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "QueryResultItem.h"
#include "OrphanedResultItemPool.h"

using std::vector, std::map, std::unordered_map, std::unordered_set;

class QueryResultItemPool {
 private:
  typedef unordered_set<QueryResultItem*> QueryResultItemSet;
  vector<QueryResultItemPtr> ownedItems;

  map<StmtValue, QueryResultItem *> stmtLookup;
  unordered_map<EntityValue, QueryResultItem *> entLookup;

  vector<QueryResultItemPtr> releaseOwned();
  void adoptStmts(const QueryResultItemPool *other,
                  QueryResultItemSet *adoptedSet,
                  QueryResultItemMapping *translationMap);
  void adoptEntities(const QueryResultItemPool *other,
                     QueryResultItemSet *adoptedSet,
                     QueryResultItemMapping *translationMap);

 public:
  QueryResultItem *getItem(const StmtValue &value);
  QueryResultItem *getItem(const EntityValue &value);

  OrphanedResultItemPoolPtr adoptPool(QueryResultItemPool *other);
};
