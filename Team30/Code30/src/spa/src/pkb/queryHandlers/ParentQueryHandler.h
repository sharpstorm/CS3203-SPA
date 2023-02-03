#pragma once

#include "../../common/Types.h"
#include "../storage/ParentStorage.h"

class ParentQueryHandler {
 public:
  explicit ParentQueryHandler(const ParentStorage* parentStore);

  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const;

 private:
  const ParentStorage* parentStore;
};
