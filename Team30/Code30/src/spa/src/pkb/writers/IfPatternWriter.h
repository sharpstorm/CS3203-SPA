#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IIfPatternWriter.h"

class IfPatternWriter : public IIfPatternWriter {
 public:
  explicit IfPatternWriter(IfPatternStorage *store);

  void addIfPattern(StmtValue stmt, EntityValue variable) override;

 private:
  IfPatternStorage *store;
};
