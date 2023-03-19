#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IWhilePatternQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "BaseQueryHandler.h"

using std::string;

class WhilePatternQueryHandler : private PkbStmtEntQueryHandler,
                                 public IWhilePatternQueryHandler {
 public:
  explicit WhilePatternQueryHandler(PkbStmtEntQueryInvoker *,
                                    WhilePatternStorage *);

  QueryResult<StmtValue, EntityValue> queryWhilePattern(StmtRef,
                                                        EntityRef) const override;

};
