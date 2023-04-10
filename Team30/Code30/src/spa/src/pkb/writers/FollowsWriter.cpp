#include "FollowsWriter.h"

FollowsWriter::FollowsWriter(FollowsStorage* store) : store(store) {}

void FollowsWriter::addFollows(StmtValue leftArg, StmtValue rightArg) {
  store->insert(leftArg, rightArg);
}
