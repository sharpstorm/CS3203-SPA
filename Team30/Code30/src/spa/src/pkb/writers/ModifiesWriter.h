#pragma once

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"

class ModifiesWriter : public IModifiesWriter {
 public:
  explicit ModifiesWriter(ModifiesStorage *, ModifiesPStorage *);

  void addModifies(StmtValue stmtNum, EntityValue variable,
                   EntityValue procedure) override;

 private:
  ModifiesStorage *modifiesStorage;
  ModifiesPStorage *modifiesPStorage;
};
