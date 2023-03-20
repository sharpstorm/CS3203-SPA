#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IFollowsTQueryHandler : virtual public PkbStmtStmtQueryHandler {
 public:
  virtual ~IFollowsTQueryHandler() = default;
  using PkbStmtStmtQueryHandler::query;
};

