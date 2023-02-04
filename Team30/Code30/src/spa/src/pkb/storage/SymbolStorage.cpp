#include "SymbolStorage.h"

SymbolStorage::SymbolStorage() {
}

void SymbolStorage::addSymbol \
    (const std::string& entityName, EntityType entityType) {
    SymbolTable[entityName] = entityType;
    // add to reverse symbol table
    ReverseSymbolTable[entityType].push_back(entityName);
}

EntityType SymbolStorage::getSymbol(const std::string& entityName) {
    return SymbolTable.at(entityName);
}

std::vector<std::string> SymbolStorage::getSymbolsOfType \
    (EntityType entityType) {
    return ReverseSymbolTable.at(entityType);
}

