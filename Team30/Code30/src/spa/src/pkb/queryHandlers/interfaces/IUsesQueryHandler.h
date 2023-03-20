#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IUsesQueryHandler
    : virtual public PkbStmtEntQueryHandler,
        virtual public PkbEntEntQueryHandler {
 public:
  virtual ~IUsesQueryHandler() = default;
  using PkbStmtEntQueryHandler::query;
  using PkbEntEntQueryHandler::query;
  virtual EntityValue getPrintDeclarations(StmtValue) const = 0;
};
