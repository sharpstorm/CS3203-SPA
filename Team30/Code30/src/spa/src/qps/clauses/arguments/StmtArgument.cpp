#include "StmtArgument.h"

StmtArgument::StmtArgument(const StmtValue &stmtNumber) :
    stmtNumber(stmtNumber) {}

StmtRef StmtArgument::toStmtRef() const {
  if (stmtNumber == NO_STMT) {
    return StmtRef{StmtType::None, INVALID_STMT};
  }
  return StmtRef{StmtType::None, stmtNumber};
}

EntityRef StmtArgument::toEntityRef() const {
  return EntityRef{EntityType::None, NO_ENT};
}

bool StmtArgument::isConstant() const {
  return true;
}
