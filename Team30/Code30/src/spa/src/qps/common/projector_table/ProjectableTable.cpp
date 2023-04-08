#include <utility>

#include "ProjectableTable.h"

ProjectableTable::ProjectableTable(bool isBooleanResult,
                                   bool booleanResult) :
    isBooleanResult(isBooleanResult),
    booleanResult(booleanResult) {}

void ProjectableTable::addResultGroup(ProjectableGroupPtr rg) {
  groupResults.push_back(std::move(rg));
}

bool ProjectableTable::getIsBooleanResult() const {
  return isBooleanResult;
}

bool ProjectableTable::getBooleanResult() const {
  return booleanResult;
}

int ProjectableTable::getResultGroupCount() const {
  return groupResults.size();
}

bool ProjectableTable::operator==(const ProjectableTable &srt) const {
  if (booleanResult != srt.booleanResult ||
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

int ProjectableTable::getFinalRowCount() const {
  if (groupResults.empty()) {
    return 0;
  }

  int resultSize = groupResults.at(0)->getRowCount();
  for (size_t i = 1; i < groupResults.size(); i++) {
    resultSize *= groupResults.at(i)->getRowCount();
  }

  return resultSize;
}

ProjectorIndex ProjectableTable::buildProjectionIndex(
    const AttributedSynonymList *synList,
    const PkbQueryHandler *pkbHandler) const {
  ProjectorIndex ret;

  for (auto it = synList->begin(); it != synList->end(); it++) {
    for (int groupId = 0; groupId < groupResults.size(); groupId++) {
      ResultTableCol col = groupResults.at(groupId)
          ->getSynonymCol(it->getName());
      if (col != ProjectableGroup::NO_COL) {
        ResultItemProjector itemProjector(pkbHandler, &(*it));
        ret.push_back(ProjectorInstruction(groupId, col, itemProjector));
      }
    }
  }

  return ret;
}

void ProjectableTable::projectTo(QPSOutputList *output,
                                 const ProjectorIndex &index) const {
  int finalRowCount = getFinalRowCount();
  vector<ResultTableRow> rowIndexes(groupResults.size(), 0);

  ProjectedValue rowOut;
  rowOut.reserve(1024);
  for (int i = 0; i < finalRowCount; i++) {
    populateIndexes(&rowIndexes, i);
    projectForRow(index, &rowIndexes, &rowOut);
    output->push_back(rowOut);
  }
}

void ProjectableTable::populateIndexes(RowIndexes *indexes,
                                       const ProjectedRow outputRow) const {
  ProjectedRow workingRow = outputRow;
  for (size_t i = 0; i < groupResults.size(); i++) {
    int groupSize = groupResults.at(i)->getRowCount();
    indexes->at(i) = workingRow % groupSize;
    workingRow /= groupSize;
  }
}

void ProjectableTable::projectForRow(const ProjectorIndex &index,
                                     const RowIndexes *row,
                                     ProjectedValue *outputCache) const {
  ResultGroupId groupId;
  ResultTableCol groupCol;
  ResultTableRow groupRow;
  outputCache->clear();

  for (size_t indexPos = 0; indexPos < index.size(); indexPos++) {
    const ProjectorInstruction &inst = index.at(indexPos);
    groupId = inst.getGroupId();
    groupCol = inst.getTableCol();
    groupRow = row->at(groupId);

    const QueryResultItem *item = groupResults.at(groupId)
        ->getEntryAt(groupRow, groupCol);

    *outputCache += inst.project(item);
    if (indexPos >= index.size() - 1) {
      continue;
    }
    *outputCache += " ";
  }
}
