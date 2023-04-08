#pragma once

#include "IPostProcessor.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StorageTypes.h"

class ParentTPostProcessor : public IPostProcessor {
 public:
  explicit ParentTPostProcessor(PKB*);

  void process() override;

 private:
  ParentTable* parentTable;
  ParentTStorage* parentTStorage;
  ParentRevTable* parentRevTable;

  void dfsParentRevTable(StmtValue, StmtValueSet&) const;
};
