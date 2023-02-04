#pragma once

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"

class FollowsQueryHandler {
 public:
  explicit FollowsQueryHandler(
      const FollowsStorage* followsStore,
      const PredicateFactory* predicateFactory,
      const StructureMappingProvider* stuctureProvider);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;

 private:
  const FollowsStorage* store;
  const PredicateFactory* predicateFactory;
  const StructureMappingProvider* structureProvider;
};
