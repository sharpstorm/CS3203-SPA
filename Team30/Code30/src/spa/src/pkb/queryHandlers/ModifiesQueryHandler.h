#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "BaseQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "PkbEntEntQueryInvoker.h"
#include "interfaces/IModifiesQueryHandler.h"

class ModifiesQueryHandler : public IModifiesQueryHandler {
 public:
  ModifiesQueryHandler(
      PkbStmtEntQueryInvoker *,
      PkbEntEntQueryInvoker *,
      ModifiesStorage *,
      ModifiesPStorage *);

  EntityValue getReadDeclarations(StmtValue) const;

 private:
  const ModifiesStorage *modifiesStorage;
};
