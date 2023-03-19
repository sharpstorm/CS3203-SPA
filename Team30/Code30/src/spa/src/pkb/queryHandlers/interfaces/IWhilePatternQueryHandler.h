#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IWhilePatternQueryHandler : virtual public PkbStmtEntQueryHandler {
 public:
  virtual ~IWhilePatternQueryHandler() = default;
  using PkbStmtEntQueryHandler::query;
};
