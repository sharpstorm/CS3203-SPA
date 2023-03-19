#pragma once

#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "common/Types.h"
#include "BaseQueryInvoker.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/predicates/EntityPredicateFactory.h"

class PkbEntEntQueryInvoker : public BaseQueryInvoker<EntityValue,
                                                      EntityType,
                                                      EntityValue,
                                                      EntityType> {
 public:
  PkbEntEntQueryInvoker(
      IEntityMappingProvider *entityMappingProvider,
      EntityPredicateFactory *predicateFactory);
};

