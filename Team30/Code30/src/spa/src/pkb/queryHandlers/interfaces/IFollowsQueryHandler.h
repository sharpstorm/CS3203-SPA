#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IFollowsQueryHandler : virtual public PkbStmtStmtQueryHandler {
 public:
  virtual ~IFollowsQueryHandler() = default;
  using PkbStmtStmtQueryHandler::query;
};
