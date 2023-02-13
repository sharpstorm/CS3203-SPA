#pragma once

#include <string>
#include "common/Types.h"
#include "interfaces/IUsesQueryHandler.h"

using std::string;

class UsesQueryHandler : public IUsesQueryHandler {
 public:
  UsesQueryHandler();

  QueryResult<int, string> queryUses(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryUses(EntityRef, EntityRef) const override;
};
