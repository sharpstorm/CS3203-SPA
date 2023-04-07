#include "CyclicProceduresValidator.h"

#include "pkb/errors/PKBError.h"

CyclicProceduresValidator::CyclicProceduresValidator(PKB* pkb)
    : procedureValues(pkb->procedureValues), callsTTable(pkb->callsTTable) {}

void CyclicProceduresValidator::validate() {
  auto procedures = procedureValues;
  for (auto it = procedures->begin(); it != procedures->end(); it++) {
    const auto& procedure = *it;
    auto allCalled = callsTTable->get(procedure);
    if (allCalled.find(procedure) != allCalled.end()) {
      throw PKBError(PKBERR_CYCLIC_PROCEDURE);
    }
  }
}
