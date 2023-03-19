#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/ICallsTQueryHandler.h"
#include "PkbEntEntQueryInvoker.h"
#include "BaseQueryHandler.h"

class CallsTQueryHandler : private PkbEntEntQueryHandler,
    public ICallsTQueryHandler {
 public:
  CallsTQueryHandler(
      PkbEntEntQueryInvoker *,
      CallsTStorage *);

  QueryResult<EntityValue, EntityValue> queryCallsStar(
      EntityRef, EntityRef) const override;
};
