#include "PkbWriter.h"

#include "AssignsWriter.h"
#include "CallsWriter.h"
#include "FollowsWriter.h"
#include "ModifiesWriter.h"
#include "ParentWriter.h"
#include "ProcedureWriter.h"
#include "StatementWriter.h"
#include "SymbolWriter.h"
#include "UsesWriter.h"
#include "PostProcessWriter.h"

PkbWriter::PkbWriter(PKB *pkb)
    : followsWriter(new FollowsWriter(pkb->followsStore)),
      parentWriter(new ParentWriter(pkb->parentStore)),
      usesWriter(new UsesWriter(pkb->usesStorage, pkb->usesPStorage)),
      modifiesWriter(new ModifiesWriter(pkb->modifiesStorage,
                                        pkb->modifiesPStorage)),
      symbolWriter(new SymbolWriter(pkb->symbolStorage)),
      statementWriter(new StatementWriter(pkb->statementStorage)),
      procedureWriter(new ProcedureWriter(pkb->procedureStorage)),
      assignsWriter(new AssignsWriter(pkb->assignStorage)),
      callsWriter(new CallsWriter(pkb->callsStorage, pkb->callStmtStorage)),
      postProcessWriter(new PostProcessWriter(pkb)) {}

void PkbWriter::addFollows(int arg1, int arg2) {
  followsWriter->addFollows(arg1, arg2);
}

void PkbWriter::addParent(int arg1, int arg2) {
  parentWriter->addParent(arg1, arg2);
}

void PkbWriter::addSymbol(string entityName, EntityType entityType) {
  symbolWriter->addSymbol(entityName, entityType);
}

void PkbWriter::addProcedure(string procedureName, int startLineNum,
                             int endLineNum) {
  procedureWriter->addProcedure(procedureName, startLineNum, endLineNum);
  // add in both first, will optimise later
  symbolWriter->addSymbol(procedureName, EntityType::Procedure);
}

void PkbWriter::addStatement(int lineNumber, StmtType stmtType) {
  statementWriter->addStatement(lineNumber, stmtType);
}

void PkbWriter::addUses(int stmtNum, string variable, string procedure) {
  usesWriter->addUses(stmtNum, variable, procedure);
}

void PkbWriter::addModifies(int stmtNum, string variable, string procedure) {
  modifiesWriter->addModifies(stmtNum, variable, procedure);
}

void PkbWriter::addAssigns(int stmtNum, PatternTrieSPtr ast) {
  assignsWriter->addAssigns(stmtNum, ast);
}

void PkbWriter::addCalls(int stmtNum, string currProcedure,
                         string calledProcedure) {
  callsWriter->addCalls(stmtNum, currProcedure, calledProcedure);
}

void PkbWriter::runPostProcessor() {
  postProcessWriter->runPostProcessor();
}
