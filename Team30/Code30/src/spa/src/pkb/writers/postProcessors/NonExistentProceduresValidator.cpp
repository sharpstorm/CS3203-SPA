#include "NonExistentProceduresValidator.h"
#include "pkb/errors/PKBError.h"

NonExistentProceduresValidator::NonExistentProceduresValidator(PKB *pkb) :
    pkb(pkb) {}

void NonExistentProceduresValidator::validate() {
  auto procedures = pkb->procedureStorage->getAllValues();
  auto allCalled = pkb->callStmtStorage->getAllValues();
  for (auto called : allCalled) {
    if (procedures.find(called) == procedures.end()) {
      throw PKBError(PKBERR_NONEXISTENT_PROCEDURE);
    }
  }
}

