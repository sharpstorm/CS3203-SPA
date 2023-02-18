#pragma once

#include "ClauseArgument.h"

class StmtArgument: public ClauseArgument {
 private:
  int stmtNumber;

 public:
  explicit StmtArgument(int stmtNumber);
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
