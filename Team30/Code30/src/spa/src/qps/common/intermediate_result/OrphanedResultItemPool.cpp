#include "OrphanedResultItemPool.h"

OrphanedResultItemPool::OrphanedResultItemPool(
    const QueryResultItemMapping &mapping):lookupMap(mapping) {}

void OrphanedResultItemPool::adopt(QueryResultItemPtr item) {
  owned.push_back(std::move(item));
}

QueryResultItem *OrphanedResultItemPool::getMappingFor(QueryResultItem *item) {
  const auto &it = lookupMap.find(item);
  if (it == lookupMap.end()) {
    return item;
  }

  return it->second;
}
