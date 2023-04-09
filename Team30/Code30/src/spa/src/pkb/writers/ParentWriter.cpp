#include "ParentWriter.h"

ParentWriter::ParentWriter(ParentStorage* store) : store(store) {}

void ParentWriter::addParent(StmtValue arg1, StmtValue arg2) {
  return store->insert(arg1, arg2);
}
