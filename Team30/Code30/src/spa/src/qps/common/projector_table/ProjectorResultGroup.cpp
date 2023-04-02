#include <utility>
#include <memory>
#include <string>

#include "ProjectorResultGroup.h"

using std::make_unique, std::to_string;

void ProjectorResultGroup::addRow(QueryResultTableRow row) {
  groupTable.push_back(std::move(row));
}

void ProjectorResultGroup::addSynonym(PQLSynonymName name) {
  int curIndex = colIdx.size();
  colMap.emplace(name, curIndex);
  colIdx.push_back(name);
}

int ProjectorResultGroup::getRowCount() const {
  return groupTable.size();
}

const QueryResultTableRow* ProjectorResultGroup::getRowAt(int idx) const {
  return &groupTable.at(idx);
}

bool ProjectorResultGroup::operator==(const ProjectorResultGroup &rg) const {
  if (colMap.size() != rg.colMap.size() ||
      groupTable.size() != groupTable.size()) {
    return false;
  }

  for (const auto& it : colMap) {
    if (rg.colMap.find(it.first) == rg.colMap.end()) {
      return false;
    }
  }

  for (int i= 0; i < groupTable.size(); i++) {
    bool isFound = false;
    for (int j = 0; j < rg.groupTable.size(); j++) {
      // Differing column lengths
      if (groupTable[i].size() != rg.groupTable[j].size()) {
        return false;
      }

      // Go through all the synonyms
      for (auto it : rg.colMap) {
        int otherIdx = it.second;
        int thisIdx = colMap.at(it.first);

        if (*groupTable[i][thisIdx] == *rg.groupTable[j][otherIdx]) {
          isFound = true;
          break;
        }
      }
      if (isFound) {
        break;
      }
    }

    if (!isFound) {
      return false;
    }
  }

  return true;
}

QueryResultItemPool *ProjectorResultGroup::getOwnedPool() {
  return &ownedItems;
}

ResultTableCol ProjectorResultGroup::getSynonymCol(const PQLSynonymName &name) const {
  auto it = colMap.find(name);
  if (it == colMap.end()) {
    return NO_COL;
  }
  return it->second;
}
