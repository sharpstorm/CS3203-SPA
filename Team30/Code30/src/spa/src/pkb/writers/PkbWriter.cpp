#include "PkbWriter.h"

PkbWriter::PkbWriter(PKB* pkb)
    : followsWriter(pkb->followsStore),
      parentWriter(pkb->parentStore),
      symbolWriter(pkb->symbolStorage),
      statementWriter(pkb->statementStorage),
      procedureWriter(pkb->procedureStorage) {}

void PkbWriter::addFollows(int arg1, int arg2) {
  followsWriter.addFollows(arg1, arg2);
}

void PkbWriter::addParent(int arg1, int arg2) {
  parentWriter.addParent(arg1, arg2);
}

void PkbWriter::addSymbol(const std::string& entityName,
                          EntityType entityType) {
  symbolWriter.addSymbol(entityName, entityType);
}

void PkbWriter::addProcedure(const std::string& procedureName, int startLineNum,
                             int endLineNum) {
  procedureWriter.addProcedure(procedureName, startLineNum, endLineNum);
}

void PkbWriter::addStatement(int lineNumber, StmtType stmtType) {
  statementWriter.addStatement(lineNumber, stmtType);
}
