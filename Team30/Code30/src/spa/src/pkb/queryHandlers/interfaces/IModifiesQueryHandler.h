#pragma once

#include <string>
#include "common/Types.h"

class IModifiesQueryHandler {
 public:
  virtual ~IModifiesQueryHandler() {}
  virtual QueryResult<int, string> queryModifies(StmtRef, EntityRef) const = 0;
  virtual QueryResult<string, string> queryModifies(EntityRef,
                                                    EntityRef) const = 0;
  virtual string getReadDeclarations(int) const = 0;
};

