#include "CyclicProceduresValidator.h"

#include "pkb/errors/PKBError.h"

CyclicProceduresValidator::CyclicProceduresValidator(PKB* pkb) : pkb(pkb) {}

void CyclicProceduresValidator::validate() {
  auto procedures = pkb->procedureValues;
  for (auto it = procedures->begin(); it != procedures->end(); it++) {
    const auto& procedure = *it;
    auto allCalled = pkb->callsTTable->get(procedure);
    if (allCalled.find(procedure) != allCalled.end()) {
      throw PKBError(PKBERR_CYCLIC_PROCEDURE);
    }
  }
}
