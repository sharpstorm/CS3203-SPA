#include "FollowsWriter.h"

FollowsWriter::FollowsWriter(FollowsStorage* store) : store(store) {}

void FollowsWriter::addFollows(StmtValue arg1, StmtValue arg2) {
  store->insert(arg1, arg2);
}
