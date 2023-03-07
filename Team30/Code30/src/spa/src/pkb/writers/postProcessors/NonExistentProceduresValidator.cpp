#include <algorithm>

#include "NonExistentProceduresValidator.h"
#include "pkb/errors/PKBError.h"

NonExistentProceduresValidator::NonExistentProceduresValidator(PKB *pkb) :
    pkb(pkb) {}

void NonExistentProceduresValidator::validate() {
  auto procedures = pkb->procedureStorage->getAllValues();
  auto allCalled = pkb->callStmtStorage->getAllValues();
  if (!std::includes(procedures.begin(),
                     procedures.end(),
                     allCalled.begin(),
                     allCalled.end())) {
    throw PKBError(PKBERR_NONEXISTENT_PROCEDURE);
  }
}

