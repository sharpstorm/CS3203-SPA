#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IFollowsTQueryHandler.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"

class FollowsTQueryHandler : private PkbStmtStmtQueryHandler,
                             public IFollowsTQueryHandler {
 public:
  FollowsTQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                       FollowsTStorage *storage);

  QueryResult<StmtValue, StmtValue> queryFollowsStar(StmtRef,
                                                     StmtRef) const override;
};
