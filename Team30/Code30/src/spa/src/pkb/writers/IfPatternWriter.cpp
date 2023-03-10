
#include "IfPatternWriter.h"

IfPatternWriter::IfPatternWriter(IfPatternStorage *store) : store(store) {}

void IfPatternWriter::addIfPattern(int stmt, string variable) {
  store->insert(stmt, variable);
}
