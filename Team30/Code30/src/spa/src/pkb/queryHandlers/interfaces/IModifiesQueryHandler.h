#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IModifiesQueryHandler
    : virtual public PkbStmtEntQueryHandler,
        virtual public PkbEntEntQueryHandler {
 public:
  virtual ~IModifiesQueryHandler() = default;
  using PkbStmtEntQueryHandler::query;
  using PkbEntEntQueryHandler::query;
  virtual EntityValue getReadDeclarations(StmtValue) const = 0;
};
