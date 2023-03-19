#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/ICallsTQueryHandler.h"
#include "PkbEntEntQueryInvoker.h"
#include "BaseQueryHandler.h"

class CallsTQueryHandler : public ICallsTQueryHandler {
 public:
  CallsTQueryHandler(
      PkbEntEntQueryInvoker *,
      CallsTStorage *);
};
