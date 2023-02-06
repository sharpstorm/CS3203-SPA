#include "SymbolStorage.h"

SymbolStorage::SymbolStorage() : SymbolTable(), ReverseSymbolTable() {}

void SymbolStorage::addSymbol(const std::string &entityName,
                              EntityType entityType) {
  SymbolTable[entityName] = entityType;
  // add to reverse symbol table
  ReverseSymbolTable[entityType].insert(entityName);
}

EntityType SymbolStorage::getSymbol(const std::string &entityName) const {
  return SymbolTable.at(entityName);
}

std::unordered_set<std::string> SymbolStorage::getSymbolsOfType(
    EntityType entityType) const {
  return ReverseSymbolTable.at(entityType);
}
