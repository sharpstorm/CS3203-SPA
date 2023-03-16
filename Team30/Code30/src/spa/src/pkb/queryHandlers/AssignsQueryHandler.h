#pragma once

#include <memory>

#include "../storage/StorageTypes.h"
#include "common/Types.h"
#include "common/pattern/PatternTrie.h"
#include "interfaces/IAssignsQueryHandler.h"

using std::shared_ptr;

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  explicit AssignsQueryHandler(const AssignStorage *);
  QueryResult<int, PatternTrie *> queryAssigns(StmtRef) const override;

 private:
  const AssignStorage *assignStore;
};
