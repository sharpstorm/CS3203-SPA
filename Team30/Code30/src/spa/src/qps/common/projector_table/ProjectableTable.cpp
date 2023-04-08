#include <utility>

#include "ProjectableTable.h"

ProjectableTable::ProjectableTable(bool staticResult) :
    staticResult(staticResult) {}

void ProjectableTable::addResultGroup(ProjectableGroupPtr rg) {
  groupResults.push_back(std::move(rg));
}

bool ProjectableTable::isStaticTrue() const {
  return staticResult;
}

bool ProjectableTable::hasGroups() const {
  return !groupResults.empty();
}

bool ProjectableTable::operator==(const ProjectableTable &srt) const {
  if (staticResult != srt.staticResult ||
      groupResults.size() != srt.groupResults.size()) {
    return false;
  }

  for (int i = 0; i < groupResults.size(); i++) {
    if (*groupResults[i] == *srt.groupResults[i]) {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

const ProjectableGroupPtrList *ProjectableTable::getGroups() const {
  return &groupResults;
}
