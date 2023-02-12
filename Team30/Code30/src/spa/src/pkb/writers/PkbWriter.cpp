#include <memory>

#include "PkbWriter.h"
#include "FollowsWriter.h"
#include "ParentWriter.h"
#include "SymbolWriter.h"
#include "StatementWriter.h"
#include "ProcedureWriter.h"
#include "UsesWriter.h"
#include "ModifiesWriter.h"

using std::make_unique;

PkbWriter::PkbWriter(PKB *pkb)
    : followsWriter(new FollowsWriter(pkb->followsStore)),
      parentWriter(new ParentWriter(pkb->parentStore)),
      usesWriter(new UsesWriter()),
      modifiesWriter(new ModifiesWriter()),
      symbolWriter(new SymbolWriter(pkb->symbolStorage)),
      statementWriter(new StatementWriter(pkb->statementStorage)),
      procedureWriter(new ProcedureWriter(pkb->procedureStorage)) {}

void PkbWriter::addFollows(int arg1, int arg2) {
  followsWriter->addFollows(arg1, arg2);
}

void PkbWriter::addParent(int arg1, int arg2) {
  parentWriter->addParent(arg1, arg2);
}

void PkbWriter::addSymbol(string entityName,
                          EntityType entityType) {
  symbolWriter->addSymbol(entityName, entityType);
}

void PkbWriter::addProcedure(string procedureName, int startLineNum,
                             int endLineNum) {
  procedureWriter->addProcedure(procedureName, startLineNum, endLineNum);
}

void PkbWriter::addStatement(int lineNumber, StmtType stmtType) {
  statementWriter->addStatement(lineNumber, stmtType);
}

void PkbWriter::addUses(int stmtNum, string variable) {}

void PkbWriter::addModifies(int stmtNum, string variable) {}
