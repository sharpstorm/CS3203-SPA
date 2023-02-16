#pragma once

#include "ClauseArgument.h"

class StmtArgument: public ClauseArgument {
 private:
  int stmtNumber;

 public:
  StmtArgument(int stmtNumber);
  StmtRef toStmtRef();
  EntityRef toEntityRef();
};
