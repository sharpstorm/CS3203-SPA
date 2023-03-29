#include "AssignsWriter.h"

AssignsWriter::AssignsWriter(AssignStorage *assignStorage)
    : assignStorage(assignStorage) {}

void AssignsWriter::addAssigns(int stmtNum, PatternTrieSPtr astNode) {
  assignStorage->insert(stmtNum, astNode);
}
