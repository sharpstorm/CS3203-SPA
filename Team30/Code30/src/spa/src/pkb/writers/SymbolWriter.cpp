#include "SymbolWriter.h"

SymbolWriter::SymbolWriter(SymbolStorage *symbolStorage)
    : symbolStorage(symbolStorage) {}

void SymbolWriter::addSymbol(string entityName,
                             EntityType entityType) {
  symbolStorage->insert(entityName, entityType);
}
