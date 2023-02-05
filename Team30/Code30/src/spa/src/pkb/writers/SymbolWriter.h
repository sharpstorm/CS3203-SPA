#pragma once

#include <string>

#include "../../common/Types.h"
#include "../storage/SymbolStorage.h"
class SymbolWriter {
 public:
  explicit SymbolWriter(SymbolStorage* symbolStorage);
  void addSymbol(const std::string&, EntityType);

 private:
  SymbolStorage* symbolStorage;
};
