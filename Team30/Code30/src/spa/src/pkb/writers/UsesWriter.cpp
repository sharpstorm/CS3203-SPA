#include "UsesWriter.h"

UsesWriter::UsesWriter(UsesStorage *usesStorage, UsesPStorage *usesPStorage)
    : usesStorage(usesStorage), usesPStorage(usesPStorage) {}

void UsesWriter::addUses(StmtValue stmtNum, EntityValue variable,
                         EntityValue procedure) {
  usesStorage->insert(stmtNum, variable);
  usesPStorage->insert(procedure, variable);
}
