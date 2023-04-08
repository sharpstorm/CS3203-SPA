#include "CallsTPostProcessor.h"

#include <utility>

using std::pair;

CallsTPostProcessor::CallsTPostProcessor(PKB* pkb)
    : callsTable(pkb->callsTable),
      callsRevTable(pkb->callsRevTable),
      callsTStorage(pkb->callsTStorage) {}

void CallsTPostProcessor::process() {
  auto it = callsTable->getTableIterator();
  pair<EntityValue, EntityValueSet> row;
  while (!(row = it->getNext()).first.empty()) {
    for (const auto& child : row.second) {
      EntityValueSet ascendants = EntityValueSet();
      dfsCallsRevTable(child, ascendants);
      for (auto p : ascendants) {
        callsTStorage->insert(p, child);
      }
    }
  }
}

void CallsTPostProcessor::dfsCallsRevTable(EntityValue target,
                                           EntityValueSet& ascendants) const {
  auto result = callsRevTable->get(target);
  for (const auto& r : result) {
    if (ascendants.find(r) != ascendants.end()) {
      continue;
    }
    ascendants.insert(r);
    dfsCallsRevTable(r, ascendants);
  }
}
