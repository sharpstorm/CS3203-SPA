#include <memory>
#include <string>

#include "ProjectorResultGroup.h"

using std::make_unique, std::to_string;

void ProjectorResultGroup::addRow(const ProjectorTableRow &row) {
  groupTable.push_back(row);
}

void ProjectorResultGroup::addSynonym(const PQLSynonymName &name) {
  ProjectorResultCol curIndex = synIndex.size();
  synIndex.emplace(name, curIndex);
}

int ProjectorResultGroup::getRowCount() const {
  return groupTable.size();
}

const QueryResultItem *ProjectorResultGroup::getEntryAt(
    const ProjectorResultRow row,
    const ProjectorResultCol col) const {
  return groupTable.at(row).at(col);
}

QueryResultItemPool *ProjectorResultGroup::getOwnedPool() {
  return &ownedItems;
}

ProjectorResultCol ProjectorResultGroup::getSynonymCol(
    const PQLSynonymName &name) const {
  const auto it = synIndex.find(name);
  if (it == synIndex.end()) {
    return NO_COL;
  }
  return it->second;
}

bool ProjectorResultGroup::operator==(const ProjectorResultGroup &rg) const {
  if (synIndex.size() != rg.synIndex.size() ||
      groupTable.size() != groupTable.size()) {
    return false;
  }

  for (const auto &it : synIndex) {
    if (rg.synIndex.find(it.first) == rg.synIndex.end()) {
      return false;
    }
  }

  for (const auto &row : groupTable) {
    if (!hasRowIn(row, rg)) {
      return false;
    }
  }

  return true;
}

bool ProjectorResultGroup::hasRowIn(const ProjectorTableRow &target,
                                    const ProjectorResultGroup &haystack)
const {
  for (int j = 0; j < haystack.groupTable.size(); j++) {
    // Differing column lengths
    if (target.size() != haystack.groupTable[j].size()) {
      return false;
    }

    // Go through all the synonyms
    for (const auto &it : haystack.synIndex) {
      ProjectorResultCol otherIdx = it.second;
      ProjectorResultCol thisIdx = synIndex.at(it.first);

      if (*target[thisIdx] == *haystack.groupTable[j][otherIdx]) {
        return true;
      }
    }
  }
  return false;
}
