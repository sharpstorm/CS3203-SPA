#include "ParentTPostProcessor.h"

#include <utility>
#include <vector>

using std::vector, std::pair;

ParentTPostProcessor::ParentTPostProcessor(PKB* pkb)
    : parentTable(pkb->parentTable),
      parentTStorage(pkb->parentTStorage),
      parentRevTable(pkb->parentRevTable) {}

void ParentTPostProcessor::process() {
  auto it = parentTable->getTableIterator();
  StmtValueSet lastChildren;
  pair<StmtValue, StmtSet> row;
  while ((row = it->getNext()).first != 0) {
    // at most two last children with same parent
    // for while, the two last children at the same stmt
    // 1. last sibling of first child (last stmt in if clause)
    auto firstChild = *(row.second.begin());
    StmtValue lastSibling = parentTStorage->getLastSibling(firstChild);
    lastChildren.insert(lastSibling);
    // 2. last child of parent (last stmt in else clause)
    auto lastChild = *(row.second.rbegin());
    lastChildren.insert(lastChild);
  }
  for (auto child : lastChildren) {
    StmtValueSet ascendants = StmtValueSet();
    dfsParentRevTable(child, ascendants);
    for (auto p : ascendants) {
      parentTStorage->insert(p, child);
    }
  }
}

void ParentTPostProcessor::dfsParentRevTable(StmtValue child,
                                             StmtValueSet& ascendants) const {
  auto result = parentRevTable->get(child);
  for (auto r : result) {
    if (ascendants.find(r) != ascendants.end()) {
      continue;
    }
    ascendants.insert(r);
    dfsParentRevTable(r, ascendants);
  }
}
