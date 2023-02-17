#include <string>
#include "UsesWriter.h"

using std::string;

UsesWriter::UsesWriter(UsesStorage *store) : store(store) {}

void UsesWriter::addUses(int stmtNum, string variable) {
  store->insert(stmtNum, variable);
}
