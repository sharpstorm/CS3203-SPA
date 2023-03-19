#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IIfPatternQueryHandler : virtual public PkbStmtEntQueryHandler {
 public:
  virtual ~IIfPatternQueryHandler() = default;
  using PkbStmtEntQueryHandler::query;
};
