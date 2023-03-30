#pragma once

#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

class FollowsQueryHandler : public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                      RelationTableManager<StmtValue, StmtValue> *storage);
};
