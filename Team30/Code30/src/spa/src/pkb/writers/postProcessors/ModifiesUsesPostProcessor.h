#pragma once

#include "IPostProcessor.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IPkbWriter.h"

class ModifiesUsesPostProcessor : public IPostProcessor {
 public:
  explicit ModifiesUsesPostProcessor(PKB*);

  void process() override;

 private:
  ProcedureValues* procedureValues;
  CallsTTable* callsTTable;
  ModifiesPTable* modifiesPTable;
  ModifiesPStorage* modifiesPStorage;
  ModifiesStorage* modifiesStorage;
  UsesPTable* usesPTable;
  UsesPStorage* usesPStorage;
  UsesStorage* usesStorage;
  StatementStorage* statementStorage;
  CallDeclarationTable* callDeclarationTable;
  ParentTStorage* parentTStorage;

  void populateProcedureAndVars();
  void populateCallStmtAndContainers();
};
