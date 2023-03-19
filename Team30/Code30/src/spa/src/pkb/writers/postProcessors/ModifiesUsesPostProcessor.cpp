#include <unordered_set>
#include <string>
#include "ModifiesUsesPostProcessor.h"

using std::unordered_set, std::string;

ModifiesUsesPostProcessor::ModifiesUsesPostProcessor(PKB *pkb) : pkb(pkb) {}

void ModifiesUsesPostProcessor::process() {
  populateProcedureAndVars();
  populateCallStmtAndContainers();
}

void ModifiesUsesPostProcessor::populateProcedureAndVars() {
  auto procedures =
      pkb->entityMappingProvider->getValuesOfType(EntityType::Procedure);
  for (const auto &procedure : procedures) {
    // get descendent procedures
    auto calledProcedures = pkb->callsTStorage->getByFirstArg(procedure);
    for (const auto &calledProcedure : calledProcedures) {
      auto modifiesVars = pkb->modifiesPStorage->getByFirstArg(calledProcedure);
      for (const auto &v : modifiesVars) {
        pkb->modifiesPStorage->insert(procedure, v);
      }
      auto usesVars = pkb->usesPStorage->getByFirstArg(calledProcedure);
      for (const auto &v : usesVars) {
        pkb->usesPStorage->insert(procedure, v);
      }
    }
  }
}

void ModifiesUsesPostProcessor::populateCallStmtAndContainers() {
  auto callStmts = pkb->structureProvider->getValuesOfType(StmtType::Call);
  for (auto &stmt : callStmts) {
    auto procedure = pkb->structureProvider->getCalledProcedure(stmt);
    auto modifiesVars = pkb->modifiesPStorage->getByFirstArg(procedure);
    auto usesVars = pkb->usesPStorage->getByFirstArg(procedure);
    // get container stmts
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
