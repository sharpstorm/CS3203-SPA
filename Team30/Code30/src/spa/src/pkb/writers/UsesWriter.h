#pragma once

#include <string>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IUsesWriter.h"

using std::string;

class UsesWriter : public IUsesWriter {
 public:
  UsesWriter();

  void addUses(int stmtNum, string variable) override;
};
