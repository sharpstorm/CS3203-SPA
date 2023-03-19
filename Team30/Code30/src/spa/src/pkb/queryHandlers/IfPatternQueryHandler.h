#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "BaseQueryHandler.h"

using std::string;

class IfPatternQueryHandler : private PkbStmtEntQueryHandler,
                              public IIfPatternQueryHandler {
 public:
  explicit IfPatternQueryHandler(PkbStmtEntQueryInvoker *,
                                 IfPatternStorage *);

  QueryResult<StmtValue, EntityValue> queryIfPattern(StmtRef,
                                                     EntityRef) const override;

};
