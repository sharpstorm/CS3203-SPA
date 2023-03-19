#include <unordered_set>
#include <string>
#include "ModifiesUsesPostProcessor.h"

using std::unordered_set, std::string;

ModifiesUsesPostProcessor::ModifiesUsesPostProcessor(PKB *pkb) : pkb(pkb) {}

void ModifiesUsesPostProcessor::process() {
  populateProcedureAndVars();
  populateCallStmtAndVars();
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

void ModifiesUsesPostProcessor::populateCallStmtAndVars() {
  auto callStmts = pkb->structureProvider->getValuesOfType(StmtType::Call);
  for (auto &stmt : callStmts) {
    auto procedure = pkb->structureProvider->getCalledProcedure(stmt);
    auto modifiesVars = pkb->modifiesPStorage->getByFirstArg(procedure);
    for (const auto &v : modifiesVars) {
      pkb->modifiesStorage->insert(stmt, v);
    }
    auto usesVars = pkb->usesPStorage->getByFirstArg(procedure);
    for (const auto &v : usesVars) {
      pkb->usesStorage->insert(stmt, v);
    }
  }
}
