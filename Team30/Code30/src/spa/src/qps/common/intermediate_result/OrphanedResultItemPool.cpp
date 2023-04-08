#include "OrphanedResultItemPool.h"

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

void OrphanedResultItemPool::insertMapping(QueryResultItem *from,
                                           QueryResultItem *to) {
  lookupMap.emplace(from, to);
}
