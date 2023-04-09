#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IParentQueryHandler.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"

class ParentQueryHandler : public IParentQueryHandler {
 public:
  ParentQueryHandler(
      PkbStmtStmtQueryInvoker *invoker,
      IStorage<StmtValue, StmtValue> *storage);
};
