#include <memory>
#include <string>

#include "ProjectorResultGroup.h"

using std::make_unique, std::to_string;

void ProjectorResultGroup::addRow(const QueryResultTableRow &row) {
  groupTable.push_back(row);
}

void ProjectorResultGroup::addSynonym(const PQLSynonymName &name) {
  int curIndex = colIdx.size();
  colMap.emplace(name, curIndex);
  colIdx.push_back(name);
}

int ProjectorResultGroup::getRowCount() const {
  return groupTable.size();
}

const QueryResultTableRow *ProjectorResultGroup::getRowAt(
    const ProjectorResultRow &idx) const {
  return &groupTable.at(idx);
}

QueryResultItemPool *ProjectorResultGroup::getOwnedPool() {
  return &ownedItems;
}

ResultTableCol ProjectorResultGroup::getSynonymCol(const PQLSynonymName &name)
const {
  auto it = colMap.find(name);
  if (it == colMap.end()) {
    return NO_COL;
  }
  return it->second;
}

bool ProjectorResultGroup::operator==(const ProjectorResultGroup &rg) const {
  if (colMap.size() != rg.colMap.size() ||
      groupTable.size() != groupTable.size()) {
    return false;
  }

  for (const auto &it : colMap) {
    if (rg.colMap.find(it.first) == rg.colMap.end()) {
      return false;
    }
  }

  for (int i = 0; i < groupTable.size(); i++) {
    if (!hasRowIn(groupTable[i], rg)) {
      return false;
    }
  }

  return true;
}

bool ProjectorResultGroup::hasRowIn(const QueryResultTableRow &target,
                                    const ProjectorResultGroup &haystack)
const {
  for (int j = 0; j < haystack.groupTable.size(); j++) {
    // Differing column lengths
    if (target.size() != haystack.groupTable[j].size()) {
      return false;
    }

    // Go through all the synonyms
    for (const auto &it : haystack.colMap) {
      ProjectorResultCol otherIdx = it.second;
      ProjectorResultCol thisIdx = colMap.at(it.first);

      if (*target[thisIdx] == *haystack.groupTable[j][otherIdx]) {
        return true;
      }
    }
  }
  return false;
}
