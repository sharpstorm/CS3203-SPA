#pragma once

#include "../../common/Types.h"
#include "../storage/ParentStorage.h"

class ParentQueryHandler {
 public:
  explicit ParentQueryHandler(ParentStorage* parentStore);

  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2);

 private:
  ParentStorage* parentStore;
};
