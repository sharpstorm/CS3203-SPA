#include "ParentWriter.h"

ParentWriter::ParentWriter(ParentStorage* store) : store(store) {}

void ParentWriter::addParent(StmtValue leftArg, StmtValue rightArg) {
  return store->insert(leftArg, rightArg);
}
