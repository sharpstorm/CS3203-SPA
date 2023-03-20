#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "BaseQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "PkbEntEntQueryInvoker.h"
#include "interfaces/IUsesQueryHandler.h"

class UsesQueryHandler : public IUsesQueryHandler {
 public:
  UsesQueryHandler(
      PkbStmtEntQueryInvoker *,
      PkbEntEntQueryInvoker *,
      UsesStorage *,
      UsesPStorage *);

  EntityValue getPrintDeclarations(StmtValue) const override;

 private:
  const UsesStorage *usesStorage;
};

