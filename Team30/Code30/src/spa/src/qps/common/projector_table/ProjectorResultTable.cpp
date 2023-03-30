#include <memory>
#include <utility>

#include "ProjectorResultTable.h"

using std::make_unique;

ProjectorResultTable::ProjectorResultTable(bool isBooleanResult,
                                           bool booleanResult) :
    isBooleanResult(isBooleanResult),
    booleanResult(booleanResult) { }

void ProjectorResultTable::addResultGroup(ResultGroupPtr rg) {
  groupResults.push_back(std::move(rg));
}

bool ProjectorResultTable::getIsBooleanResult() {
  return isBooleanResult;
}

bool ProjectorResultTable::getBooleanResult() {
  return booleanResult;
}

int ProjectorResultTable::getResultGroupCount() {
  return groupResults.size();
}
ProjectorResultGroup* ProjectorResultTable::getResultGroup(int idx) {
  return groupResults.at(idx).get();
}

bool ProjectorResultTable::operator==(const ProjectorResultTable &srt) const {
  if (booleanResult != srt.booleanResult ||
      groupResults.size() != srt.groupResults.size()) {
    return false;
  }

  for (int i=0; i < groupResults.size(); i++) {
    if (*groupResults[i].get() == *srt.groupResults[i].get())  {
      continue;
    } else {
      return false;
    }
  }

  return true;
}
