#include "CallsTPostProcessor.h"

#include <utility>

using std::pair;

CallsTPostProcessor::CallsTPostProcessor(PKB* pkb) : pkb(pkb) {}

void CallsTPostProcessor::process() {
  auto it = pkb->callsTable->getTableIterator();
  pair<EntityValue, EntitySet> row;
  while (!(row = it->getNext()).first.empty()) {
    for (const auto& child : row.second) {
      EntityValueSet ascendants = EntityValueSet();
      dfsCallsRevTable(child, ascendants);
      for (auto p : ascendants) {
        this->pkb->callsTStorage->insert(p, child);
      }
    }
  }
}

void CallsTPostProcessor::dfsCallsRevTable(EntityValue target,
                                           EntityValueSet& ascendants) const {
  auto result = pkb->callsRevTable->get(target);
  for (const auto& r : result) {
    if (ascendants.find(r) != ascendants.end()) {
      continue;
    }
    ascendants.insert(r);
    dfsCallsRevTable(r, ascendants);
  }
}
