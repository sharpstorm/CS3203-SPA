#pragma once

#include <string>
#include <memory>

#include "common/Types.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"
#include "pkb/writers/interfaces/IParentWriter.h"
#include "pkb/writers/interfaces/IUsesWriter.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"
#include "pkb/writers/interfaces/IStatementWriter.h"
#include "pkb/writers/interfaces/ISymbolWriter.h"
#include "pkb/writers/interfaces/IPkbWriter.h"

using std::unique_ptr;

class PkbWriter : public IPkbWriter {
 public:
  explicit PkbWriter(PKB *pkb);

  void addFollows(int stmtNum, int stmtNum2) override;
  void addParent(int stmtNum, int stmtNum2) override;
  void addSymbol(string name, EntityType type) override;
  void addProcedure(string name, int start, int end) override;
  void addStatement(int, StmtType) override;
  void addUses(int stmtNum, string variable) override;
  void addModifies(int stmtNum, string variable) override;

 private:
  unique_ptr<IFollowsWriter> followsWriter;
  unique_ptr<IParentWriter> parentWriter;
  unique_ptr<IUsesWriter> usesWriter;
  unique_ptr<IModifiesWriter> modifiesWriter;
  unique_ptr<ISymbolWriter> symbolWriter;
  unique_ptr<IStatementWriter> statementWriter;
  unique_ptr<IProcedureWriter> procedureWriter;
};
