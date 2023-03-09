
#include "WhilePatternWriter.h"

WhilePatternWriter::WhilePatternWriter(WhilePatternStorage *store)
    : store(store) {}

void WhilePatternWriter::addWhilePattern(int stmt, string variable) {
  store->insert(stmt, variable);
}
