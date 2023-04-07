#include "ModifiesUsesPostProcessor.h"

#include <string>
#include <unordered_set>

using std::unordered_set, std::string;

ModifiesUsesPostProcessor::ModifiesUsesPostProcessor(PKB *pkb) : pkb(pkb) {}

void ModifiesUsesPostProcessor::process() {
  populateProcedureAndVars();
  populateCallStmtAndContainers();
}

void ModifiesUsesPostProcessor::populateProcedureAndVars() {
  auto procedures = pkb->procedureValues;
  for (auto it = procedures->begin(); it != procedures->end(); it++) {
    const auto &procedure = *it;

    // get descendent procedures
    auto calledProcedures = pkb->callsTTable->get(procedure);
    for (const auto &calledProcedure : calledProcedures) {
      auto modifiesVars = pkb->modifiesPTable->get(calledProcedure);
      for (const auto &v : modifiesVars) {
        pkb->modifiesPStorage->insert(procedure, v);
      }
      auto usesVars = pkb->usesPTable->get(calledProcedure);
      for (const auto &v : usesVars) {
        pkb->usesPStorage->insert(procedure, v);
      }
    }
  }
}

void ModifiesUsesPostProcessor::populateCallStmtAndContainers() {
  auto callStmts = pkb->statementStorage->getStatementsOfType(StmtType::Call);
  for (auto &stmt : callStmts) {
    auto procedure = pkb->callDeclarationTable->get(stmt);
    auto modifiesVars = pkb->modifiesPTable->get(procedure);
    auto usesVars = pkb->usesPTable->get(procedure);
    // get container stmts by last sibling
    auto allStmts = pkb->parentTStorage->getBySecondArg(stmt);
    allStmts.insert(stmt);
    for (const auto &s : allStmts) {
      for (const auto &v : modifiesVars) {
        pkb->modifiesStorage->insert(s, v);
      }
      for (const auto &v : usesVars) {
        pkb->usesStorage->insert(s, v);
      }
    }
  }
}
