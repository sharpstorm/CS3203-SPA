#pragma once

#include <memory>

#include "common/Types.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/interfaces/IAssignsWriter.h"
#include "pkb/writers/interfaces/ICFGsWriter.h"
#include "pkb/writers/interfaces/IConstantWriter.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"
#include "pkb/writers/interfaces/IParentWriter.h"
#include "pkb/writers/interfaces/IPkbWriter.h"
#include "pkb/writers/interfaces/IPostProcessWriter.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"
#include "pkb/writers/interfaces/IStatementWriter.h"
#include "pkb/writers/interfaces/ISymbolWriter.h"
#include "pkb/writers/interfaces/IUsesWriter.h"
#include "pkb/writers/interfaces/IVariableWriter.h"

using std::unique_ptr;

class PkbWriter : public IPkbWriter {
 public:
  explicit PkbWriter(PKB *pkb);

  void addAssigns(StmtValue stmtNum, PatternTrieSPtr ast) override;
  void addFollows(StmtValue stmtNum, StmtValue stmtNum2) override;
  void addParent(StmtValue stmtNum, StmtValue stmtNum2) override;
  EntityIdx addVariable(EntityValue name) override;
  EntityIdx addConstant(EntityValue name) override;
  void addProcedure(EntityValue name, StmtValue start, StmtValue end) override;
  void addStatement(StmtValue, StmtType) override;
  void addUses(StmtValue stmtNum, EntityValue variable,
               EntityValue procedure) override;
  void addModifies(StmtValue stmtNum, EntityValue variable,
                   EntityValue procedure) override;
  void addCalls(StmtValue stmtNum, EntityValue currProcedure,
                EntityValue calledProcedure) override;
  void addIfPattern(StmtValue stmtNum, EntityValue variable) override;
  void addWhilePattern(StmtValue stmtNum, EntityValue variable) override;
  void addCFGs(EntityValue name, CFGSPtr cfg) override;

  void runPostProcessor() override;

 private:
  unique_ptr<IFollowsWriter> followsWriter;
  unique_ptr<IParentWriter> parentWriter;
  unique_ptr<IUsesWriter> usesWriter;
  unique_ptr<IModifiesWriter> modifiesWriter;
  unique_ptr<IStatementWriter> statementWriter;
  unique_ptr<IProcedureWriter> procedureWriter;
  unique_ptr<IAssignsWriter> assignsWriter;
  unique_ptr<ICallsWriter> callsWriter;
  unique_ptr<IIfPatternWriter> ifPatternWriter;
  unique_ptr<IWhilePatternWriter> whilePatternWriter;
  unique_ptr<IPostProcessWriter> postProcessWriter;
  unique_ptr<IVariableWriter> variableWriter;
  unique_ptr<IConstantWriter> constantWriter;
  unique_ptr<ICFGsWriter> cfgsWriter;
};
