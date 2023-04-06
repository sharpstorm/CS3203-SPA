#include "OverrideTable.h"

StmtRef OverrideTable::transformArg(const PQLSynonymName &name,
                                    const StmtRef &ref) const {
  return transformArgInternal(name, ref);
}

EntityRef OverrideTable::transformArg(const PQLSynonymName &name,
                                      const EntityRef &ref) const {
  return transformArgInternal(name, ref);
}

bool OverrideTable::contains(const PQLSynonymName &name) const {
  return table.find(name) != table.end();
}

void OverrideTable::insert(const PQLSynonymName &name,
                           const OverrideTransformer &transformer) {
  table.emplace(name, transformer);
}

OverrideTransformer OverrideTable::get(const PQLSynonymName &name) const {
  return table.at(name);
}

template<class T>
T OverrideTable::transformArgInternal(const PQLSynonymName &name,
                                      const T &ref) const {
  auto it = table.find(name);
  if (it == table.end()) {
    return ref;
  }

  return it->second.transformArg(ref);
}
