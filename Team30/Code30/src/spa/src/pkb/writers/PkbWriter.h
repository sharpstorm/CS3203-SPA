#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/storage/PKB.h"
#include "FollowsWriter.h"
#include "ParentWriter.h"
#include "ProcedureWriter.h"
#include "StatementWriter.h"
#include "SymbolWriter.h"
#include "pkb/writers/interfaces/IPkbWriter.h"

class PkbWriter : public IPkbWriter {
 public:
  explicit PkbWriter(PKB *pkb);

  void addFollows(int arg1, int arg2) override;
  void addParent(int arg1, int arg2) override;
  void addSymbol(string, EntityType) override;
  void addProcedure(string, int, int) override;
  void addStatement(int, StmtType) override;

 private:
  FollowsWriter followsWriter;
  ParentWriter parentWriter;
  SymbolWriter symbolWriter;
  StatementWriter statementWriter;
  ProcedureWriter procedureWriter;
};
