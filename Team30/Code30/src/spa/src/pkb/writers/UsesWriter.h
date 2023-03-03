#pragma once

#include <string>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IUsesWriter.h"

using std::string;

class UsesWriter : public IUsesWriter {
 public:
  explicit UsesWriter(UsesStorage *, UsesPStorage *);

  void addUses(int stmtNum, string variable, string procedure) override;

 private:
  UsesStorage *usesStorage;
  UsesPStorage *usesPStorage;
};
