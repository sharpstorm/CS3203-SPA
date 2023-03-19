#include <memory>
#include <utility>

#include "SynonymResultTable.h"

using std::make_unique;

SynonymResultTable::SynonymResultTable(bool isBooleanResult,
                                       bool booleanResult) :
    isBooleanResult(isBooleanResult),
    booleanResult(booleanResult) { }

void SynonymResultTable::addResultGroup(ResultGroupPtr rg) {
  groupResults.push_back(std::move(rg));
}

bool SynonymResultTable::getIsBooleanResult() {
  return isBooleanResult;
}

bool SynonymResultTable::getBooleanResult() {
  return booleanResult;
}

int SynonymResultTable::getResultGroupCount() {
  return groupResults.size();
}
ResultGroup* SynonymResultTable::getResultGroup(int idx) {
  return groupResults.at(idx).get();
}

bool SynonymResultTable::operator==(const SynonymResultTable &srt) const {
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
