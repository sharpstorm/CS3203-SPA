#include <memory>
#include <utility>

#include "SynonymResultTable.h"
#include "ResultCoalescer.h"

using std::make_unique;

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping,
                                       bool booleanResult) :
    synonymMapping(mapping), booleanResult(booleanResult) { }

void SynonymResultTable::extractResults(PQLQueryResult *result,
                                        vector<PQLSynonymName> syns) {
  ResultGroup* resultGroup = new ResultGroup();
  // Add synonyms to the new ResultGroup
  for (PQLSynonymName name : syns) {
    resultGroup->addSynonym(name);
  }

  IntersectSet<int> rowsToTake = getUniqueRows(result, &syns);

  for (int rowIdx : rowsToTake) {
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
IntersectSet<int> SynonymResultTable::getUniqueRows(
    PQLQueryResult* result, vector<PQLSynonymName>* syns) {
  IntersectSet<int> rowsToTake;
  IntersectSet<int> ignoreRows;
  int rowCounts = result->getRowCount();
  for (int i = 0; i < rowCounts; i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }
    IntersectSetPtr<int> currentIgnoreRows = make_unique<IntersectSet<int>>();

    rowsToTake.insert(i);
    for (const PQLSynonymName& syn : *syns) {
      ResultTableCol colIdx = result->getSynonymCol(syn);
      QueryResultTableRow* currRow = result->getTableRowAt(i);
      RowSet* rows = result->getRowsWithValue(colIdx,
                                              currRow->at(colIdx).get());
      if (currentIgnoreRows->empty()) {
        currentIgnoreRows->insert(rows->begin(), rows->end());
      } else {
        currentIgnoreRows = SetUtils::intersectSet(currentIgnoreRows.get(),
                                                   rows);
      }
    }

    ignoreRows.insert(currentIgnoreRows->begin(), currentIgnoreRows->end());
  }

  return rowsToTake;
}
