#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "BaseQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "PkbEntEntQueryInvoker.h"
#include "interfaces/IUsesQueryHandler.h"

class UsesQueryHandler
    : private PkbStmtEntQueryHandler,
      private PkbEntEntQueryHandler,
      public IUsesQueryHandler {
 public:
  UsesQueryHandler(PkbStmtEntQueryInvoker *,
                   PkbEntEntQueryInvoker *,
                   UsesStorage *,
                   UsesPStorage *);
  QueryResult<StmtValue, EntityValue> queryUses(StmtRef,
                                                EntityRef) const override;
  QueryResult<EntityValue, EntityValue> queryUses(EntityRef,
                                                  EntityRef) const override;
  EntityValue getPrintDeclarations(StmtValue) const override;

 private:
  const UsesStorage *usesStorage;
};

