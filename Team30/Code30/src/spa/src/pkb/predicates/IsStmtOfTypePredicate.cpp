#include "IsStmtOfTypePredicate.h"

IsStmtOfTypePredicate::IsStmtOfTypePredicate(StructureMappingProvider* p, int x)
    : provider(p), x(x) {}

bool IsStmtOfTypePredicate::operator()(int y) {
  return provider->isStatementOfType(x, stmtType);
}
