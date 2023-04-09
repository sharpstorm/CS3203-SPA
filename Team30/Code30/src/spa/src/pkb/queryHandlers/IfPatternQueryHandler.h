#pragma once

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "PkbStmtEntQueryInvoker.h"
#include "BaseQueryHandler.h"

class IfPatternQueryHandler : public IIfPatternQueryHandler {
 public:
  explicit IfPatternQueryHandler(
      PkbStmtEntQueryInvoker *, IfPatternStorage *);
};
