#pragma once

#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "common/Types.h"
#include "BaseQueryInvoker.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/predicates/StmtPredicateFactory.h"

class PkbStmtStmtQueryInvoker : public BaseQueryInvoker<StmtValue,
                                                        StmtType,
                                                        StmtValue,
                                                        StmtType> {
 public:
  PkbStmtStmtQueryInvoker(
      IStructureMappingProvider *provider,
      StmtPredicateFactory *predicateFactory);
};

