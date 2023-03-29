#include "QueryResultItemPool.h"

#include <memory>
#include <utility>

using std::make_unique;

QueryResultItem *QueryResultItemPool::getItem(const StmtValue &value) {
  const auto &it = stmtLookup.find(value);
  if (it != stmtLookup.end()) {
    return it->second;
  }

  ownedItems.push_back(make_unique<QueryResultItem>(value));
  QueryResultItem* itemPtr = ownedItems.back().get();
  stmtLookup.emplace(value, itemPtr);

  return itemPtr;
}

QueryResultItem *QueryResultItemPool::getItem(const EntityValue &value) {
  const auto &it = entLookup.find(value);
  if (it != entLookup.end()) {
    return it->second;
  }

  ownedItems.push_back(make_unique<QueryResultItem>(value));
  QueryResultItem* itemPtr = ownedItems.back().get();
  entLookup.emplace(value, itemPtr);

  return itemPtr;
}

vector<QueryResultItemPtr> QueryResultItemPool::releaseOwned() {
  return std::move(ownedItems);
}

OrphanedResultItemPoolPtr QueryResultItemPool::adoptPool(
    QueryResultItemPool *other) {
  QueryResultItemMapping translationMap;
  QueryResultItemSet adoptedPtrs;
  adoptStmts(other, &adoptedPtrs, &translationMap);
  adoptEntities(other, &adoptedPtrs, &translationMap);

  OrphanedResultItemPoolPtr orphanedPtrs =
      make_unique<OrphanedResultItemPool>(translationMap);
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
                                     QueryResultItemMapping *translationMap) {
  for (const auto &it : other->stmtLookup) {
    auto mySamePtr = stmtLookup.find(it.first);
    if (mySamePtr != stmtLookup.end()) {
      translationMap->emplace(it.second, mySamePtr->second);
      continue;
    }
    stmtLookup.emplace(it.first, it.second);
    adoptedSet->insert(it.second);
  }
}

void QueryResultItemPool::adoptEntities(
    const QueryResultItemPool *other,
    QueryResultItemSet *adoptedSet,
    QueryResultItemMapping *translationMap) {
  for (const auto &it : other->entLookup) {
    auto mySamePtr = entLookup.find(it.first);
    if (mySamePtr != entLookup.end()) {
      translationMap->emplace(it.second, mySamePtr->second);
      continue;
    }
    entLookup.emplace(it.first, it.second);
    adoptedSet->insert(it.second);
  }
}
