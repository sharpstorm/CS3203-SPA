#include "ModifiesWriter.h"

ModifiesWriter::ModifiesWriter(ModifiesStorage *modifiesStorage,
                               ModifiesPStorage *modifiesPStorage)
    : modifiesStorage(modifiesStorage), modifiesPStorage(modifiesPStorage) {}

void ModifiesWriter::addModifies(StmtValue stmtNum, EntityValue variable,
                                 EntityValue procedure) {
  modifiesStorage->insert(stmtNum, variable);
  modifiesPStorage->insert(procedure, variable);
}
