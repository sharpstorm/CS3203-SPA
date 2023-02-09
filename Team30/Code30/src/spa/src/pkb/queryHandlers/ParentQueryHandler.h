#pragma once

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"
#include "interfaces/IParentQueryHandler.h"

class ParentQueryHandler : public IParentQueryHandler {
 public:
  ParentQueryHandler(const ParentStorage *store,
                     const PredicateFactory *predicateFactory,
                     const IStructureMappingProvider *stuctureProvider);

  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const;
  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const;

 private:
  const ParentStorage *store;
  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;
};
