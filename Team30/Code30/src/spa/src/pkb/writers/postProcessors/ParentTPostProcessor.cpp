#include "ParentTPostProcessor.h"

#include <vector>

using std::vector;

ParentTPostProcessor::ParentTPostProcessor(PKB* pkb) : pkb(pkb) {}

void ParentTPostProcessor::process() {
  auto parentTable = pkb->parentTable;
  StmtValueSet lastChildren;
  auto parentTStorage = pkb->parentTStorage;
  pkb::Function<StmtValue, StmtSet> func = [&lastChildren, parentTStorage](
                                               const StmtValue& key,
                                               const StmtSet& values) {
    // at most two last children with same parent
    // for while, the two last children at the same stmt
    // 1. last sibling of first child (last stmt in if clause)
    auto firstChild = *(values.begin());
    StmtValue lastSibling = parentTStorage->getLastSibling(firstChild);
    lastChildren.insert(lastSibling);
    // 2. last child of parent (last stmt in else clause)
    auto lastChild = *(values.rbegin());
    lastChildren.insert(lastChild);
  };
  parentTable->forEach(func, nullptr);
  for (auto child : lastChildren) {
    StmtValueSet ascendants = StmtValueSet();
    dfsParentRevTable(child, ascendants);
    for (auto p : ascendants) {
      pkb->parentTStorage->insert(p, child);
    }
  }
}

void ParentTPostProcessor::dfsParentRevTable(StmtValue child,
                                             StmtValueSet& ascendants) const {
  auto result = pkb->parentRevTable->get(child);
  for (auto r : result) {
    if (ascendants.find(r) != ascendants.end()) {
      continue;
    }
    ascendants.insert(r);
    dfsParentRevTable(r, ascendants);
  }
}
