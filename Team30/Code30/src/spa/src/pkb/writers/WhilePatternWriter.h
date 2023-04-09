#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IWhilePatternWriter.h"

class WhilePatternWriter : public IWhilePatternWriter {
 public:
  explicit WhilePatternWriter(WhilePatternStorage *store);

  void addWhilePattern(StmtValue stmt, EntityValue variable) override;

 private:
  WhilePatternStorage *store;
};
