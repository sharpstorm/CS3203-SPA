#include "SymbolWriter.h"

SymbolWriter::SymbolWriter(SymbolStorage* symbolStorage)
    : symbolStorage(symbolStorage) {}

void SymbolWriter::addSymbol(const std::string& entityName,
                             EntityType entityType) {
  symbolStorage->addSymbol(entityName, entityType);
}
