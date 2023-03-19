#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class ICallsQueryHandler : virtual public PkbEntEntQueryHandler {
 public:
  virtual ~ICallsQueryHandler() = default;
  using PkbEntEntQueryHandler::query;
};
