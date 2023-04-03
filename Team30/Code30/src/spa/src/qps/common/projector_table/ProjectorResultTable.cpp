#include <utility>

#include "ProjectorResultTable.h"

ProjectorResultTable::ProjectorResultTable(bool isBooleanResult,
                                           bool booleanResult) :
    isBooleanResult(isBooleanResult),
    booleanResult(booleanResult) {}

void ProjectorResultTable::addResultGroup(ResultGroupPtr rg) {
  groupResults.push_back(std::move(rg));
}

bool ProjectorResultTable::getIsBooleanResult() const {
  return isBooleanResult;
}

bool ProjectorResultTable::getBooleanResult() const {
  return booleanResult;
}

int ProjectorResultTable::getResultGroupCount() const {
  return groupResults.size();
}

bool ProjectorResultTable::operator==(const ProjectorResultTable &srt) const {
  if (booleanResult != srt.booleanResult ||
      groupResults.size() != srt.groupResults.size()) {
    return false;
  }

  for (int i = 0; i < groupResults.size(); i++) {
    if (*groupResults[i].get() == *srt.groupResults[i].get()) {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

int ProjectorResultTable::getFinalRowCount() const {
  if (groupResults.size() == 0) {
    return 0;
  }

  int resultSize = groupResults.at(0)->getRowCount();
  for (int i = 1; i < groupResults.size(); i++) {
    resultSize *= groupResults.at(i)->getRowCount();
  }

  return resultSize;
}

ProjectorIndex ProjectorResultTable::buildProjectionIndex(
    const AttributedSynonymList *synList,
    const PkbQueryHandler *pkbHandler) const {
  ProjectorIndex ret;

  for (auto it = synList->begin(); it != synList->end(); it++) {
    for (int groupId = 0; groupId < groupResults.size(); groupId++) {
      ResultTableCol col = groupResults.at(groupId)
          ->getSynonymCol(it->getName());
      if (col != ProjectorResultGroup::NO_COL) {
        ResultItemProjector itemProjector(pkbHandler, &(*it));
        ret.push_back(ProjectorInstruction(groupId, col, itemProjector));
      }
    }
  }

  return ret;
}

void ProjectorResultTable::projectTo(QPSOutputList *output,
                                     const ProjectorIndex &index) const {
  int finalRowCount = getFinalRowCount();
  vector<int> rowIndexes(groupResults.size(), 0);

  ProjectedValue rowOut;
  rowOut.reserve(1024);
  for (int i = 0; i < finalRowCount; i++) {
    populateIndexes(&rowIndexes, i);
    projectForRow(index, &rowIndexes, &rowOut);
    output->push_back(rowOut);
  }
}

void ProjectorResultTable::populateIndexes(RowIndexes *indexes,
                                           const int &outputRow) const {
  int workingRow = outputRow;
  for (int i = 0; i < groupResults.size(); i++) {
    int groupSize = groupResults.at(i)->getRowCount();
    indexes->at(i) = workingRow % groupSize;
    workingRow /= groupSize;
  }
}

void ProjectorResultTable::projectForRow(const ProjectorIndex &index,
                                         const RowIndexes *row,
                                         ProjectedValue *outputCache) const {
  int groupRow;
  outputCache->clear();
  for (int indexPos = 0; indexPos < index.size(); indexPos++) {
    const ProjectorInstruction &inst = index.at(indexPos);
    groupRow = row->at(inst.getGroupId());
    QueryResultItem *item = groupResults.at(inst.getGroupId())
        ->getRowAt(groupRow)
        ->at(inst.getTableCol());
    *outputCache += inst.project(item);

    if (indexPos >= index.size() - 1) {
      continue;
    }
    *outputCache += " ";
  }
}
