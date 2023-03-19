#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IParentTQueryHandler : virtual public PkbStmtStmtQueryHandler {
 public:
  virtual ~IParentTQueryHandler() {}
  using PkbStmtStmtQueryHandler::query;
};
