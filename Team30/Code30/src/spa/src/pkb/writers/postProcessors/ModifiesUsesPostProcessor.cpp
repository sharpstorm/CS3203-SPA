#include "ModifiesUsesPostProcessor.h"

#include <string>
#include <unordered_set>

using std::unordered_set, std::string;

ModifiesUsesPostProcessor::ModifiesUsesPostProcessor(PKB *pkb)
    : procedureValues(pkb->procedureValues),
      callsTTable(pkb->callsTTable),
      modifiesPTable(pkb->modifiesPTable),
      modifiesPStorage(pkb->modifiesPStorage),
      modifiesStorage(pkb->modifiesStorage),
      usesPTable(pkb->usesPTable),
      usesPStorage(pkb->usesPStorage),
      usesStorage(pkb->usesStorage),
      statementStorage(pkb->statementStorage),
      callDeclarationTable(pkb->callDeclarationTable),
      parentTStorage(pkb->parentTStorage) {}

void ModifiesUsesPostProcessor::process() {
  populateProcedureAndVars();
  populateCallStmtAndContainers();
}

void ModifiesUsesPostProcessor::populateProcedureAndVars() {
  auto procedures = procedureValues;
  for (auto it = procedures->begin(); it != procedures->end(); it++) {
    const auto &procedure = *it;

    // get descendent procedures
    auto calledProcedures = callsTTable->get(procedure);
    for (const auto &calledProcedure : calledProcedures) {
      auto modifiesVars = modifiesPTable->get(calledProcedure);
      for (const auto &v : modifiesVars) {
        modifiesPStorage->insert(procedure, v);
      }
      auto usesVars = usesPTable->get(calledProcedure);
      for (const auto &v : usesVars) {
        usesPStorage->insert(procedure, v);
      }
    }
  }
}

void ModifiesUsesPostProcessor::populateCallStmtAndContainers() {
  auto callStmts = statementStorage->getStatementsOfType(StmtType::Call);
  for (auto &stmt : callStmts) {
    auto procedure = callDeclarationTable->get(stmt);
    auto modifiesVars = modifiesPTable->get(procedure);
    auto usesVars = usesPTable->get(procedure);
    // get container stmts by last sibling
    auto allStmts = parentTStorage->getBySecondArg(stmt);
    allStmts.insert(stmt);
    for (const auto &s : allStmts) {
      for (const auto &v : modifiesVars) {
        modifiesStorage->insert(s, v);
      }
      for (const auto &v : usesVars) {
        usesStorage->insert(s, v);
      }
    }
  }
}
