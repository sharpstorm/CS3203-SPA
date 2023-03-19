#pragma once

#include "pkb/storage/StorageTypes.h"
#include "common/Types.h"
#include "common/pattern/PatternTrie.h"
#include "interfaces/IAssignsQueryHandler.h"

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  explicit AssignsQueryHandler(const AssignStorage *);
  QueryResult<StmtValue, PatternTrie *> queryAssigns(StmtRef) const override;

 private:
  const AssignStorage *assignStore;
};
