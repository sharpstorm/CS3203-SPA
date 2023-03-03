#include <string>
#include "ModifiesWriter.h"

using std::string;

ModifiesWriter::ModifiesWriter(ModifiesStorage *modifiesStorage,
                               ModifiesPStorage *modifiesPStorage)
    : modifiesStorage(modifiesStorage), modifiesPStorage(modifiesPStorage) {}

void ModifiesWriter::addModifies(int stmtNum,
                                 string variable,
                                 string procedure) {
  modifiesStorage->insert(stmtNum, variable);
  modifiesPStorage->insert(procedure, variable);
}
