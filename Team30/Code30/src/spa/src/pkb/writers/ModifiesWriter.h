#pragma once

#include <string>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"

using std::string;

class ModifiesWriter : public IModifiesWriter {
 public:
  ModifiesWriter();

  void addModifies(int stmtNum, string variable) override;
};
