#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"

class FollowsWriter : public IFollowsWriter {
 public:
  explicit FollowsWriter(FollowsStorage *followsStore);

  void addFollows(StmtValue leftArg, StmtValue rightArg) override;

 private:
  FollowsStorage *store;
};
