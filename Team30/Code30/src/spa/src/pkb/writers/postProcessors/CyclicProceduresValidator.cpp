#include "CyclicProceduresValidator.h"
#include "pkb/errors/PKBError.h"

CyclicProceduresValidator::CyclicProceduresValidator(PKB *pkb) : pkb(pkb) {}

void CyclicProceduresValidator::validate() {
  auto procedures = pkb->procedureStorage->getAllValues();
  for (auto procedure : procedures) {
    auto allCalled = pkb->callsStorage->getByFirstArgT(procedure);
    for (auto called : allCalled) {
      if (called == procedure) {
        throw PKBError(PKBERR_CYCLIC_PROCEDURE);
      }
    }
  }
}
