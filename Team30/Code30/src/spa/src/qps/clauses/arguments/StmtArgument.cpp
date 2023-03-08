#include "StmtArgument.h"

StmtArgument::StmtArgument(int stmtNumber): stmtNumber(stmtNumber) {
}

StmtRef StmtArgument::toStmtRef() {
  return StmtRef{StmtType::None, stmtNumber};
}

EntityRef StmtArgument::toEntityRef() {
  return EntityRef{EntityType::None, ""};
}
