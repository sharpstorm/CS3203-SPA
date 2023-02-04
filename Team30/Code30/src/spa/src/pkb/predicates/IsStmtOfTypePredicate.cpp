#include "IsStmtOfTypePredicate.h"

IsStmtOfTypePredicate::IsStmtOfTypePredicate(const StructureMappingProvider* p,
                                             StmtType stmtType)
    : provider(p), stmtType(stmtType) {}

bool IsStmtOfTypePredicate::operator()(int stmt) const {
  return provider->isStatementOfType(stmt, stmtType);
}
