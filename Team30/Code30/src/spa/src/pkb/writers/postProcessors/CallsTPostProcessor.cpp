#include "CallsTPostProcessor.h"
CallsTPostProcessor::CallsTPostProcessor(PKB* pkb) : pkb(pkb) {}

void CallsTPostProcessor::process() {
  pkb::Function<EntityValue, EntitySet> func = [this](const EntityValue& key,
                                                      const EntitySet& values) {
    for (auto child : values) {
      EntityValueSet ascendants = EntityValueSet();
      dfsCallsRevTable(child, ascendants);
      for (auto p : ascendants) {
        this->pkb->callsTStorage->insert(p, child);
      }
    }
  };
  pkb->callsTable->forEach(func, nullptr);
}

void CallsTPostProcessor::dfsCallsRevTable(EntityValue taget,
                                           EntityValueSet& ascendants) const {
  auto result = pkb->callsRevTable->get(taget);
  for (auto r : result) {
    if (ascendants.find(r) != ascendants.end()) {
      continue;
    }
    ascendants.insert(r);
    dfsCallsRevTable(r, ascendants);
  }
}
