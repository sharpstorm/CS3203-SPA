#pragma once

#include <string>
#include "../storage/SymbolStorage.h"
#include "../../common/Types.h"
class SymbolWriter {
 public:
    explicit SymbolWriter(SymbolStorage* symbolStorage);
    void addSymbol(const std::string& , EntityType);

 private:
    SymbolStorage* symbolStorage;
};
