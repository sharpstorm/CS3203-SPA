#pragma once

#include "ClauseArgument.h"

class StmtArgument : public ClauseArgument {
 private:
  StmtValue stmtNumber;

 public:
  explicit StmtArgument(const StmtValue &stmtNumber);
  StmtRef toStmtRef() const override;
  EntityRef toEntityRef() const override;
  bool isConstant() const override;
};
