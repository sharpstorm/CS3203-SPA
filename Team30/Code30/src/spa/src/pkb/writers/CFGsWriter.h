#pragma once

#include <memory>
#include <string>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ICFGsWriter.h"

using std::string;

class CFGsWriter : public ICFGsWriter {
 public:
  explicit CFGsWriter(CFGStorage *cfgStorage);
  void addCFGs(string arg1, CFGSPtr arg2) override;

 private:
  CFGStorage *cfgStorage;
};
