#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider() {}

bool StructureMappingProvider::isStatementOfType(int s,
                                                 StmtType stmtType) const {
  return false;
}

unordered_set<int> StructureMappingProvider::getStatementsOfType(
    StmtType stmtType) const {
  return unordered_set<int>();
}
