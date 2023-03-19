#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "pkb/queryHandlers/interfaces/IParentTQueryHandler.h"

class ParentTQueryHandler : public IParentTQueryHandler {
 public:
  ParentTQueryHandler(
      PkbStmtStmtQueryInvoker *invoker,
      ParentTStorage *storage);
};
