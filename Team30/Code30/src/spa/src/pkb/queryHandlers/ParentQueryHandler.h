#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IParentQueryHandler.h"
#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"

class ParentQueryHandler : private PkbStmtStmtQueryHandler,
                           public IParentQueryHandler {
 public:
  ParentQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                     ParentStorage *storage);
  QueryResult<StmtValue, StmtValue> queryParent(StmtRef,
                                                StmtRef) const override;
};
