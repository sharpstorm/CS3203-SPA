#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class ICallsTQueryHandler : virtual public PkbEntEntQueryHandler {
 public:
  virtual ~ICallsTQueryHandler() = default;
  using PkbEntEntQueryHandler::query;
};
