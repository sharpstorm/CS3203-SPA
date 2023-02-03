#pragma once

#include "../../common/Types.h"
#include "../storage/PKB.h"
#include "FollowsWriter.h"
#include "ParentWriter.h"

class PkbWriter {
 public:
  explicit PkbWriter(PKB* pkb);

  void addFollows(int arg1, int arg2);
  void addParent(int arg1, int arg2);

 private:
  FollowsWriter followsWriter;
  ParentWriter parentWriter;
};
