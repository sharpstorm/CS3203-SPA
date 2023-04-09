
#include "WhilePatternWriter.h"

WhilePatternWriter::WhilePatternWriter(WhilePatternStorage *store)
    : store(store) {}

void WhilePatternWriter::addWhilePattern(StmtValue stmt, EntityValue variable) {
  store->insert(stmt, variable);
}
