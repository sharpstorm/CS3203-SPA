#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IParentTQueryHandler {
 public:
  virtual ~IParentTQueryHandler() = default;
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      StmtRef *leftArg, StmtRef *rightArg) const = 0;
};
