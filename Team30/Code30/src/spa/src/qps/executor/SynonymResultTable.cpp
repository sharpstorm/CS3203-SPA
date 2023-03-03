#include <utility>
#include "common/SetUtils.h"
#include "SynonymResultTable.h"
#include "ResultCoalescer.h"

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping,
                                       bool booleanResult) :
    synonymMapping(mapping), booleanResult(booleanResult) { }

void SynonymResultTable::extractResults(PQLQueryResult *result,
                                        vector<PQLSynonymName> syns) {
  // Add synonyms to the new ResultGroup
  ResultGroup* resultGroup = new ResultGroup();
  IntersectSetPtr<int> rowsToExtract;
  IntersectSet<int>* rowsToTake = new unordered_set<int>();

  int rowCounts = result->getRowCount();
  for (int i=0; i < syns.size(); i++) {
    PQLSynonymName syn = syns[i];
    resultGroup->addSynonym(syn);

    unordered_set<int> ignoreRows;
    ResultTableCol colIdx = result->getSynonymCol(syn);
    for (int j=0; j < rowCounts; j++) {
      if (ignoreRows.find(j) != ignoreRows.end()) {
        continue;
      }

      rowsToTake->insert(j);
      QueryResultTableRow* currRow = result->getTableRowAt(j);
      RowSet* rows = result->getRowsWithValue(colIdx,
                                              currRow->at(colIdx).get());
      ignoreRows.insert(rows->begin(), rows->end());
    }

    if (i == 0) {
      rowsToExtract = IntersectSetPtr<int>(rowsToTake);
    } else {
      rowsToExtract = SetUtils::intersectSet(rowsToExtract.get(), rowsToTake);
    }
  }

  for (int rowIdx : *rowsToExtract) {
    QueryResultTableRow* tableRow = result->getTableRowAt(rowIdx);
    QueryResultTableRow newRow{};
    for (const PQLSynonymName& syn : syns) {
      ResultTableCol tableCol = result->getSynonymCol(syn);
      newRow.push_back(std::move(tableRow->at(tableCol)));
    }

    resultGroup->addRow(std::move(newRow));
  }
  groupResults.push_back(unique_ptr<ResultGroup>(resultGroup));
}

bool SynonymResultTable::getBooleanResult() {
  return booleanResult;
}
bool SynonymResultTable::hasTargetSynonyms() {
  return !synonymMapping->empty();
}

int SynonymResultTable::getResultGroupCount() {
  return groupResults.size();
}
ResultGroup* SynonymResultTable::getResultGroup(int idx) {
  return groupResults.at(idx).get();
}
bool SynonymResultTable::operator==(const SynonymResultTable &srt) const {
  if (booleanResult != srt.booleanResult ||
      *synonymMapping != *srt.synonymMapping ||
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
