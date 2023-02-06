#pragma once

#include <string>

#include "../../common/Types.h"
#include "../storage/StatementStorage.h"
class StatementWriter {
 public:
  explicit StatementWriter(StatementStorage* statementStorage);
  void addStatement(int, StmtType);

 private:
  StatementStorage* statementStorage;
};
