#include "AssignsWriter.h"

AssignsWriter::AssignsWriter(AssignStorage *assignStorage)
    : assignStorage(assignStorage) {}

void AssignsWriter::addAssigns(StmtValue stmtNum, PatternTrieSPtr astNode) {
  assignStorage->insert(stmtNum, astNode);
}
