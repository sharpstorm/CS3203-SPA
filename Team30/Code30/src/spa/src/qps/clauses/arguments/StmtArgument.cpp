#include "StmtArgument.h"

StmtArgument::StmtArgument(const StmtValue &stmtNumber) :
    stmtNumber(stmtNumber) {}

StmtRef StmtArgument::toStmtRef() const {
  return StmtRef{StmtType::None, stmtNumber};
}

EntityRef StmtArgument::toEntityRef() const {
  return EntityRef{EntityType::None, ""};
}

bool StmtArgument::isConstant() const {
  return true;
}
