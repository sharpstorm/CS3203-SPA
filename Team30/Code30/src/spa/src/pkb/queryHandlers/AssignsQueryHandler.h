#pragma once

#include <memory>

#include "common/Types.h"
#include "../storage/StorageTypes.h"
#include "interfaces/IAssignsQueryHandler.h"

using std::shared_ptr;

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  AssignsQueryHandler(const AssignStorage *assignStore);
  QueryResult<int, shared_ptr<IASTNode>> queryAssigns(StmtRef) const override;

 private:
  const AssignStorage *assignStore;
};
