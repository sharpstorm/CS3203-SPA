#pragma once

#include <memory>

#include "common/Types.h"
#include "common/pattern/PatternTrie.h"
#include "interfaces/IAssignsQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::make_unique;
using std::unique_ptr;

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  explicit AssignsQueryHandler(const AssignStorage *);
  QueryResultPtr<StmtValue, PatternTrie *> queryAssigns(
      StmtRef) const override;

 private:
  const AssignStorage *assignStore;
};
