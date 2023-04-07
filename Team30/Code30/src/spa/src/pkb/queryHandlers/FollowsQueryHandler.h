#pragma once

#include "PkbStmtStmtQueryInvoker.h"
#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/storage/IStorage.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/StorageTypes.h"

class FollowsQueryHandler : public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                      RelationTableManager<StmtValue, StmtValue> *storage);
};
