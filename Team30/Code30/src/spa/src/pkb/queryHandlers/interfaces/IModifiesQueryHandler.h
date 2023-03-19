#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IModifiesQueryHandler {
 public:
  virtual ~IModifiesQueryHandler() {}
  virtual QueryResult<StmtValue, EntityValue> queryModifies(
      StmtRef, EntityRef) const = 0;
  virtual QueryResult<EntityValue, EntityValue> queryModifies(
      EntityRef, EntityRef) const = 0;
  virtual EntityValue getReadDeclarations(StmtValue) const = 0;
};
