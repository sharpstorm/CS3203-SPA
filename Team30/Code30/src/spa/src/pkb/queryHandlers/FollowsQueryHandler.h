#pragma once

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"
#include "interfaces/IFollowsQueryHandler.h"

class FollowsQueryHandler : public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(const FollowsStorage *store,
                      const PredicateFactory *predicateFactory,
                      const IStructureMappingProvider *stuctureProvider);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;
  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const;

 private:
  const FollowsStorage *store;
  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;
};
