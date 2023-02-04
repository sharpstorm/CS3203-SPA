#include "PkbWriter.h"

PkbWriter::PkbWriter(PKB* pkb)
    : followsWriter(pkb->followsStore), parentWriter(pkb->parentStore) {}

void PkbWriter::addFollows(int arg1, int arg2) {}

void PkbWriter::addParent(int arg1, int arg2) {}
