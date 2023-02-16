#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/ISymbolWriter.h"

using std::string;

class SymbolWriter : public ISymbolWriter {
 public:
  explicit SymbolWriter(SymbolStorage *symbolStorage);
  void addSymbol(string, EntityType) override;

 private:
  SymbolStorage *symbolStorage;
};
