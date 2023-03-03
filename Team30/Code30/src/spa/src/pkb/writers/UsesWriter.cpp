#include <string>
#include "UsesWriter.h"

using std::string;

UsesWriter::UsesWriter(UsesStorage *usesStorage, UsesPStorage *usesPStorage)
    : usesStorage(usesStorage), usesPStorage(usesPStorage) {}

void UsesWriter::addUses(int stmtNum, string variable, string procedure) {
  usesStorage->insert(stmtNum, variable);
  usesPStorage->insert(procedure, variable);
}
