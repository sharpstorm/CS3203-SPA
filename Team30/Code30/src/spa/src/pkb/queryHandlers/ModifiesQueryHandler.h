#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "BaseQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "PkbEntEntQueryInvoker.h"
#include "interfaces/IModifiesQueryHandler.h"

class ModifiesQueryHandler
    : private PkbStmtEntQueryHandler,
      private PkbEntEntQueryHandler,
      public IModifiesQueryHandler {
 public:
  ModifiesQueryHandler(PkbStmtEntQueryInvoker *,
                       PkbEntEntQueryInvoker *,
                       ModifiesStorage *,
                       ModifiesPStorage *);
  QueryResult<StmtValue, EntityValue> queryModifies(StmtRef,
                                                    EntityRef) const override;
  QueryResult<EntityValue, EntityValue> queryModifies(EntityRef,
                                                      EntityRef) const override;
  EntityValue getReadDeclarations(StmtValue) const override;

 private:
  const ModifiesStorage *modifiesStorage;
};
