#pragma once

#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "common/Types.h"
#include "BaseQueryInvoker.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/predicates/StmtPredicateFactory.h"
#include "pkb/predicates/EntityPredicateFactory.h"

class PkbStmtEntQueryInvoker : public BaseQueryInvoker<StmtValue,
                                                       StmtType,
                                                       EntityValue,
                                                       EntityType> {
 public:
  PkbStmtEntQueryInvoker(
      IStructureMappingProvider *,
      StmtPredicateFactory *,
      EntityPredicateFactory *);
};

