#pragma once

#include <string>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"

using std::string;

class ModifiesWriter : public IModifiesWriter {
 public:
  explicit ModifiesWriter(ModifiesStorage *, ModifiesPStorage *);

  void addModifies(int stmtNum, string variable, string procedure) override;

 private:
  ModifiesStorage *modifiesStorage;
  ModifiesPStorage *modifiesPStorage;
};
