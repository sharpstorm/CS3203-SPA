#pragma once

#include <memory>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ICFGsWriter.h"

class CFGsWriter : public ICFGsWriter {
 public:
  explicit CFGsWriter(CFGStorage *cfgStorage);
  void addCFGs(EntityValue arg1, CFGSPtr arg2) override;

 private:
  CFGStorage *cfgStorage;
};
