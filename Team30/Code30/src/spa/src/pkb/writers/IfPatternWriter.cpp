
#include "IfPatternWriter.h"

IfPatternWriter::IfPatternWriter(IfPatternStorage *store) : store(store) {}

void IfPatternWriter::addIfPattern(StmtValue stmt, EntityValue variable) {
  store->insert(stmt, variable);
}
