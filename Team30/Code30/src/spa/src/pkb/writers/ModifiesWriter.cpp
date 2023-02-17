#include <string>
#include "ModifiesWriter.h"

using std::string;

ModifiesWriter::ModifiesWriter(ModifiesStorage *store) : store(store) {}

void ModifiesWriter::addModifies(int stmtNum, string variable) {
  store->insert(stmtNum, variable);
}
