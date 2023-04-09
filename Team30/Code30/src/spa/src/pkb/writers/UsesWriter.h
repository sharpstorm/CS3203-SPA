#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IUsesWriter.h"

class UsesWriter : public IUsesWriter {
 public:
  explicit UsesWriter(UsesStorage *, UsesPStorage *);

  void addUses(StmtValue stmtNum, EntityValue variable,
               EntityValue procedure) override;

 private:
  UsesStorage *usesStorage;
  UsesPStorage *usesPStorage;
};
