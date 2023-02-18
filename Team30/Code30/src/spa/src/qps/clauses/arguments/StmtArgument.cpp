#include "StmtArgument.h"
#include "qps/errors/QPSParserSyntaxError.h"

StmtArgument::StmtArgument(int stmtNumber): stmtNumber(stmtNumber) {
}

StmtRef StmtArgument::toStmtRef() {
  return StmtRef{StmtType::None, stmtNumber};
}

EntityRef StmtArgument::toEntityRef() {
  throw QPSParserSyntaxError("Invalid call to toEntityRef");
}