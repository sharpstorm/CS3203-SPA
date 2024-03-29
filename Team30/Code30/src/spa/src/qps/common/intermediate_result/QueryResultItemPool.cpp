#include "QueryResultItemPool.h"

#include <memory>
#include <utility>

using std::make_unique;

QueryResultItem *QueryResultItemPool::getItem(const StmtValue &value) {
  return lookupOrEmplace(&stmtLookup, value);
}

QueryResultItem *QueryResultItemPool::getItem(const EntityValue &value) {
  return lookupOrEmplace(&entLookup, value);
}

QueryResultItemPool::OwnedResultItemList QueryResultItemPool::releaseOwned() {
  return std::move(ownedItems);
}

OrphanedResultItemPoolPtr QueryResultItemPool::adoptPool(
    QueryResultItemPool *other) {
  OrphanedResultItemPoolPtr orphanedPtrs =
      make_unique<OrphanedResultItemPool>();
  QueryResultItemSet adoptedPtrs;
  adoptStmts(other, &adoptedPtrs, orphanedPtrs.get());
  adoptEntities(other, &adoptedPtrs, orphanedPtrs.get());

  vector<QueryResultItemPtr> otherItems = other->releaseOwned();
  for (QueryResultItemPtr &it : otherItems) {
    if (adoptedPtrs.find(it.get()) != adoptedPtrs.end()) {
      ownedItems.push_back(std::move(it));
    } else {
      orphanedPtrs->adopt(std::move(it));
    }
  }

  return std::move(orphanedPtrs);
}

void QueryResultItemPool::adoptStmts(const QueryResultItemPool *other,
                                     QueryResultItemSet *adoptedSet,
                                     OrphanedResultItemPool *orphanPool) {
  adoptFrom(&stmtLookup, other->stmtLookup, adoptedSet, orphanPool);
}

void QueryResultItemPool::adoptEntities(
    const QueryResultItemPool *other,
    QueryResultItemSet *adoptedSet,
    OrphanedResultItemPool *orphanPool) {
  adoptFrom(&entLookup, other->entLookup, adoptedSet, orphanPool);
}

template<class ValueType, class MapType>
QueryResultItem *QueryResultItemPool::lookupOrEmplace(MapType *lookupMap,
                                                      const ValueType &value) {
  const auto &it = lookupMap->find(value);
  if (it != lookupMap->end()) {
    return it->second;
  }

  ownedItems.push_back(make_unique<QueryResultItem>(value));
  QueryResultItem *itemPtr = ownedItems.back().get();
  lookupMap->emplace(value, itemPtr);

  return itemPtr;
}

template<class MapType>
void QueryResultItemPool::adoptFrom(MapType *lookupMap,
                                    const MapType &otherMap,
                                    QueryResultItemSet *adoptedSet,
                                    OrphanedResultItemPool *orphanPool) {
  for (const auto &it : otherMap) {
    auto mySamePtr = lookupMap->find(it.first);
    if (mySamePtr != lookupMap->end()) {
      orphanPool->insertMapping(it.second, mySamePtr->second);
      continue;
    }
    lookupMap->emplace(it.first, it.second);
    adoptedSet->insert(it.second);
  }
}
