#include "ParentWriter.h"

ParentWriter::ParentWriter(ParentStorage* store) : store(store) {}

void ParentWriter::addParent(int arg1, int arg2) {
  return store->insert(arg1, arg2);
}
