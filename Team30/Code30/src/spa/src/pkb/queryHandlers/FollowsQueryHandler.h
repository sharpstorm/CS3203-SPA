#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"

class FollowsQueryHandler : private PkbStmtStmtQueryHandler,
                            public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                      FollowsStorage *storage);
  QueryResult<StmtValue, StmtValue> queryFollows(StmtRef,
                                                 StmtRef) const override;
};
