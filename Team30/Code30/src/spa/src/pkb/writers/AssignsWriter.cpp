#include "AssignsWriter.h"

AssignsWriter::AssignsWriter(AssignStorage *assignStorage)
    : assignStorage(assignStorage) {}

void AssignsWriter::addAssigns(int stmtNum, shared_ptr<IASTNode> astNode) {
  assignStorage->set(stmtNum, astNode);
}
