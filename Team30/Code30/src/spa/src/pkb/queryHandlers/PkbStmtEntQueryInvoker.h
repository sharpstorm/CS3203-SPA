#pragma once

#include "BaseQueryInvoker.h"
#include "common/Types.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/predicates/EntityPredicateFactory.h"
#include "pkb/predicates/StmtPredicateFactory.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

class PkbStmtEntQueryInvoker
    : public BaseQueryInvoker<StmtValue, StmtType, EntityValue, EntityType> {
 public:
  PkbStmtEntQueryInvoker(IStructureMappingProvider *, IEntityMappingProvider *,
                         StmtPredicateFactory *, EntityPredicateFactory *);
};
