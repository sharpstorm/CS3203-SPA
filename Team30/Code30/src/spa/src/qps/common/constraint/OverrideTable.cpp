#include "OverrideTable.h"

StmtRef OverrideTable::transformArg(const PQLSynonymName &name, StmtRef ref) {
  if (!contains(name)) {
    return ref;
  }

  return table.at(name).transformArg(ref);
}

EntityRef OverrideTable::transformArg(const PQLSynonymName &name, EntityRef ref) {
  if (!contains(name)) {
    return ref;
  }

  return table.at(name).transformArg(ref);
}

bool OverrideTable::contains(const PQLSynonymName &name) {
  return table.find(name) != table.end();
}

void OverrideTable::insert(const PQLSynonymName &name,
                           const OverrideTransformer &transformer) {
  table.emplace(name, transformer);
}

OverrideTransformer OverrideTable::get(const PQLSynonymName &name) {
  return table.at(name);
}
