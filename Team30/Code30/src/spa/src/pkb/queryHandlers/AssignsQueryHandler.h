#pragma once

#include <memory>

#include "common/Types.h"
#include "../storage/StorageTypes.h"
#include "interfaces/IAssignsQueryHandler.h"
#include "common/pattern/PatternTrie.h"

using std::shared_ptr;

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  explicit AssignsQueryHandler(const AssignStorage *);
  QueryResult<int, PatternTrie*> queryAssigns(StmtRef) const override;

 private:
  const AssignStorage *assignStore;
};
