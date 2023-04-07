#include "NonExistentProceduresValidator.h"

#include "pkb/errors/PKBError.h"

NonExistentProceduresValidator::NonExistentProceduresValidator(PKB *pkb)
    : procAndCallsStorage(pkb->procAndCallsStorage) {}

void NonExistentProceduresValidator::validate() {
  if (procAndCallsStorage->hasUndefinedProcedures()) {
    throw PKBError(PKBERR_NONEXISTENT_PROCEDURE);
  }
}
