#include "StructureMappingProvider.h"

bool StructureMappingProvider::isStatementOfType(int s,
                                                 StmtType stmtType) const {
  return stmtType == StmtType::Assign;
}
