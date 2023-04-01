#include "NonExistentProceduresValidator.h"

#include "pkb/errors/PKBError.h"

NonExistentProceduresValidator::NonExistentProceduresValidator(PKB *pkb)
    : pkb(pkb) {}

void NonExistentProceduresValidator::validate() {
  if (pkb->procAndCallsStorage->hasUndefinedProcedures()) {
    throw PKBError(PKBERR_NONEXISTENT_PROCEDURE);
  }
}
