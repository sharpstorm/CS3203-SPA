#pragma once

#include <string>
#include "common/Types.h"
#include "interfaces/IModifiesQueryHandler.h"

using std::string;

class ModifiesQueryHandler : public IModifiesQueryHandler {
 public:
  ModifiesQueryHandler();

  QueryResult<int, string> queryModifies(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryModifies(EntityRef,
                                            EntityRef) const override;
};
