#pragma once

#include "IPostProcessor.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StorageTypes.h"

class CallsTPostProcessor : public IPostProcessor {
 public:
  explicit CallsTPostProcessor(PKB*);

  void process() override;

 private:
  CallsTable* callsTable;
  CallsRevTable* callsRevTable;
  CallsTStorage* callsTStorage;

  void dfsCallsRevTable(EntityValue, EntityValueSet&) const;
};
