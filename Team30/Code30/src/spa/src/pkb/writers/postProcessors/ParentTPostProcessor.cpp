#include "ParentTPostProcessor.h"

#include <vector>

using std::vector;

ParentTPostProcessor::ParentTPostProcessor(PKB* pkb) : pkb(pkb) {}

void ParentTPostProcessor::process() {
  auto parentTable = pkb->parentTable;
  StmtValueSet lastChildren;
  auto followsTable = pkb->followsTable;
  pkb::Function<StmtValue, StmtSet> func = [&lastChildren, followsTable](
                                               const StmtValue& key,
                                               const StmtSet& values) {
    // note: row values should not be empty, so firstChild and lastChild
    // shld be valid get last sibling of firstChild
    auto firstChild = *(values.begin());
    auto siblings = followsTable->get(firstChild);
    StmtValue lastSibling;
    if (siblings.empty()) {
      // may not have last sibling. last sibling is first child
      lastSibling = firstChild;
    } else {
      lastSibling = *siblings.rbegin();
    }
    lastChildren.insert(lastSibling);
    // get last child of parent
    auto lastChild = *(values.rbegin());
    lastChildren.insert(lastChild);
  };
  parentTable->forEach(func, nullptr);
  for (auto child : lastChildren) {
    StmtSet ascendants = StmtSet();
    dfsParentRevTable(child, ascendants);
    for (auto p : ascendants) {
      pkb->parentTStorage->insert(p, child);
    }
  }

}

void ParentTPostProcessor::dfsParentRevTable(StmtValue child,
                                             StmtSet& ascendants) const {
  auto result = pkb->parentRevTable->get(child);
  for (auto r : result) {
    ascendants.insert(r);
    dfsParentRevTable(r, ascendants);
  }
}
