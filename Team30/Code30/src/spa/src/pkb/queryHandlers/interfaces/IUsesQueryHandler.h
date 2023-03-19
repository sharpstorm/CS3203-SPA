#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IUsesQueryHandler {
 public:
  virtual ~IUsesQueryHandler() {}
  virtual QueryResult<StmtValue, EntityValue> queryUses(StmtRef,
                                                        EntityRef) const = 0;
  virtual QueryResult<EntityValue, EntityValue> queryUses(EntityRef,
                                                          EntityRef) const = 0;
  virtual EntityValue getPrintDeclarations(StmtValue) const = 0;
};
