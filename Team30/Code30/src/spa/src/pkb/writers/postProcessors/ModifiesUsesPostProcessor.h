#pragma once

#include "IPostProcessor.h"
#include "pkb/writers/interfaces/IPkbWriter.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/PKB.h"

class ModifiesUsesPostProcessor : public IPostProcessor {
 public:
  explicit ModifiesUsesPostProcessor(PKB *);

  void process() override;

 private:
  PKB *pkb;

  void populateProcedureAndVars();
  void populateCallStmtAndContainers();
};
