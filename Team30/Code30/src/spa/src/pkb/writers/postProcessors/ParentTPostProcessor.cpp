#include "ParentTPostProcessor.h"

#include <vector>

using std::vector;

ParentTPostProcessor::ParentTPostProcessor(PKB* pkb) : pkb(pkb) {}

void ParentTPostProcessor::process() {
  auto parentTable = pkb->parentTable;
  StmtValueSet lastChildren;
  for (auto it = parentTable->begin(); it != parentTable->end(); it++) {
    // note: row values should not be empty, so firstChild and lastChild shld be
    // valid get last sibling of firstChild
    auto firstChild = *(it->second.begin());
    auto lastSibling = *(pkb->followsTable->get(firstChild).end());
    // may not have last sibling. last sibling is first child
    if (lastSibling == 0) {
      lastSibling = firstChild;
    }
    lastChildren.insert(lastSibling);
    // get last child of parent
    auto lastChild = *(it->second.rbegin());
    lastChildren.insert(lastChild);
  }
  for (auto child : lastChildren) {
    vector<StmtValue> ascendants;
    dfsParentRevTable(child, ascendants);
    pkb->parentTStorage();
  }
}

void ParentTPostProcessor::dfsParentRevTable(
    StmtValue child, vector<StmtValue>& ascendants) const {
  auto result = pkb->parentRevTable->get(child);
  for (auto r : result) {
    ascendants.push_back(r);
    dfsParentRevTable(r, ascendants);
  }
}
