#pragma once

#include "PkbStmtStmtQueryInvoker.h"
#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

class FollowsQueryHandler : public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                      FollowsTableManager *storage);
};
