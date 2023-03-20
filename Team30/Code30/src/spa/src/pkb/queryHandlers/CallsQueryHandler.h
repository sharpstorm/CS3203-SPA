#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "interfaces/ICallsQueryHandler.h"
#include "PkbEntEntQueryInvoker.h"
#include "BaseQueryHandler.h"

class CallsQueryHandler : public ICallsQueryHandler {
 public:
  CallsQueryHandler(
      PkbEntEntQueryInvoker *,
      CallsStorage *);
};
