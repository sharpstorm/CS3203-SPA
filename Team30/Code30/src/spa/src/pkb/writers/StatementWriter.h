#pragma once

#include <string>
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IStatementWriter.h"

class StatementWriter : public IStatementWriter {
 public:
  explicit StatementWriter(StatementStorage *statementStorage);
  void addStatement(int, StmtType) override;

 private:
  StatementStorage *statementStorage;
};
