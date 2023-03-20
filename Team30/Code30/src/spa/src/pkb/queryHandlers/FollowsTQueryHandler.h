#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IFollowsTQueryHandler.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"

class FollowsTQueryHandler : public IFollowsTQueryHandler {
 public:
  FollowsTQueryHandler(
      PkbStmtStmtQueryInvoker *invoker,
      FollowsTStorage *storage);
};
