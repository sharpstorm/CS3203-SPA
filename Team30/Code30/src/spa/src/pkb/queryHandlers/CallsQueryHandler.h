#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "interfaces/ICallsQueryHandler.h"
#include "PkbEntEntQueryInvoker.h"
#include "BaseQueryHandler.h"

class CallsQueryHandler : private PkbEntEntQueryHandler,
                          public ICallsQueryHandler {
 public:
  CallsQueryHandler(PkbEntEntQueryInvoker *,
                    CallsStorage *);

  QueryResult<EntityValue, EntityValue> queryCalls(EntityRef,
                                                   EntityRef) const override;
};
