#include "PkbWriter.h"

#include "AssignsWriter.h"
#include "CFGsWriter.h"
#include "CallsWriter.h"
#include "ConstantWriter.h"
#include "FollowsWriter.h"
#include "IfPatternWriter.h"
#include "ModifiesWriter.h"
#include "ParentWriter.h"
#include "PostProcessWriter.h"
#include "ProcedureWriter.h"
#include "StatementWriter.h"
#include "UsesWriter.h"
#include "VariableWriter.h"
#include "WhilePatternWriter.h"

PkbWriter::PkbWriter(PKB *pkb)
    : followsWriter(new FollowsWriter(pkb->followsStorage)),
      parentWriter(new ParentWriter(pkb->parentStorage)),
      usesWriter(new UsesWriter(pkb->usesStorage, pkb->usesPStorage)),
      modifiesWriter(
          new ModifiesWriter(pkb->modifiesStorage, pkb->modifiesPStorage)),
      statementWriter(new StatementWriter(pkb->statementStorage)),
      procedureWriter(new ProcedureWriter(pkb->procAndCallsStorage)),
      assignsWriter(new AssignsWriter(pkb->assignStorage)),
      ifPatternWriter(new IfPatternWriter(pkb->ifPatternStorage)),
      whilePatternWriter(new WhilePatternWriter(pkb->whilePatternStorage)),
      callsWriter(new CallsWriter(pkb->callsStorage, pkb->procAndCallsStorage)),
      postProcessWriter(new PostProcessWriter(pkb)),
      variableWriter(new VariableWriter(pkb->variableStorage)),
      constantWriter(new ConstantWriter(pkb->constantStorage)),
      cfgsWriter(new CFGsWriter(pkb->cfgStorage)) {}

void PkbWriter::addFollows(StmtValue arg1, StmtValue arg2) {
  followsWriter->addFollows(arg1, arg2);
}

void PkbWriter::addParent(StmtValue arg1, StmtValue arg2) {
  parentWriter->addParent(arg1, arg2);
}

void PkbWriter::addProcedure(EntityValue procedureName, StmtValue startLineNum,
                             StmtValue endLineNum) {
  procedureWriter->addProcedure(procedureName, startLineNum, endLineNum);
}

void PkbWriter::addStatement(StmtValue lineNumber, StmtType stmtType) {
  statementWriter->addStatement(lineNumber, stmtType);
}

void PkbWriter::addUses(StmtValue stmtNum, EntityValue variable,
                        EntityValue procedure) {
  usesWriter->addUses(stmtNum, variable, procedure);
}

void PkbWriter::addModifies(StmtValue stmtNum, EntityValue variable,
                            EntityValue procedure) {
  modifiesWriter->addModifies(stmtNum, variable, procedure);
}

void PkbWriter::addAssigns(StmtValue stmtNum, PatternTrieSPtr ast) {
  assignsWriter->addAssigns(stmtNum, ast);
}

void PkbWriter::addCalls(StmtValue stmtNum, EntityValue currProcedure,
                         EntityValue calledProcedure) {
  callsWriter->addCalls(stmtNum, currProcedure, calledProcedure);
}

void PkbWriter::addIfPattern(StmtValue stmtNum, EntityValue variable) {
  ifPatternWriter->addIfPattern(stmtNum, variable);
}

void PkbWriter::addWhilePattern(StmtValue stmtNum, EntityValue variable) {
  whilePatternWriter->addWhilePattern(stmtNum, variable);
}

void PkbWriter::runPostProcessor() { postProcessWriter->runPostProcessor(); }

EntityIdx PkbWriter::addVariable(EntityValue name) {
  return variableWriter->addVariable(name);
}

EntityIdx PkbWriter::addConstant(EntityValue name) {
  return constantWriter->addConstant(name);
}

void PkbWriter::addCFGs(EntityValue name, CFGSPtr cfg) {
  cfgsWriter->addCFGs(name, cfg);
}
