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
  typedef unordered_set<QueryResultItem *> QueryResultItemSet;
  typedef map<StmtValue, QueryResultItem *> StmtLookupMap;
  typedef unordered_map<EntityValue, QueryResultItem *> EntityLookupMap;
  typedef vector<QueryResultItemPtr> OwnedResultItemList;

  OwnedResultItemList ownedItems;

  StmtLookupMap stmtLookup;
  EntityLookupMap entLookup;

  OwnedResultItemList releaseOwned();
  void adoptStmts(const QueryResultItemPool *other,
                  QueryResultItemSet *adoptedSet,
                  QueryResultItemMapping *translationMap);
  void adoptEntities(const QueryResultItemPool *other,
                     QueryResultItemSet *adoptedSet,
                     QueryResultItemMapping *translationMap);

  template<class ValueType, class MapType>
  QueryResultItem *lookupOrEmplace(MapType *lookupMap,
                                   const ValueType &value);

  template<class MapType>
  void adoptFrom(MapType *lookupMap,
                 const MapType &other,
                 QueryResultItemSet *adoptedSet,
                 QueryResultItemMapping *translationMap);

 public:
  QueryResultItem *getItem(const StmtValue &value);
  QueryResultItem *getItem(const EntityValue &value);

  OrphanedResultItemPoolPtr adoptPool(QueryResultItemPool *other);
};
