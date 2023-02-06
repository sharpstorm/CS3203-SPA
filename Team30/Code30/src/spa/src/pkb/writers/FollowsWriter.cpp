#include "FollowsWriter.h"

FollowsWriter::FollowsWriter(FollowsStorage* store) : store(store) {}

void FollowsWriter::addFollows(int arg1, int arg2) {
  store->insert(arg1, arg2);
}
