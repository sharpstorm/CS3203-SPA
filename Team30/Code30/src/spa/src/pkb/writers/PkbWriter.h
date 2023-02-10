#pragma once

#include <string>

#include "../../common/Types.h"
#include "../storage/PKB.h"
#include "FollowsWriter.h"
#include "ParentWriter.h"
#include "ProcedureWriter.h"
#include "StatementWriter.h"
#include "SymbolWriter.h"

class PkbWriter {
 public:
  explicit PkbWriter(PKB* pkb);

  virtual void addFollows(int arg1, int arg2);
  void addParent(int arg1, int arg2);
  void addSymbol(const std::string&, EntityType);
  void addProcedure(const std::string&, int, int);
  void addStatement(int, StmtType);

 private:
  FollowsWriter followsWriter;
  ParentWriter parentWriter;
  SymbolWriter symbolWriter;
  StatementWriter statementWriter;
  ProcedureWriter procedureWriter;
};
