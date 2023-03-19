#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IParentQueryHandler : virtual public PkbStmtStmtQueryHandler {
 public:
  virtual ~IParentQueryHandler() = default;
  using PkbStmtStmtQueryHandler::query;
};
