#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"

class FollowsWriter : public IFollowsWriter {
 public:
  explicit FollowsWriter(FollowsStorage *followsStore);

  void addFollows(int arg1, int arg2) override;

 private:
  FollowsStorage *store;
};
