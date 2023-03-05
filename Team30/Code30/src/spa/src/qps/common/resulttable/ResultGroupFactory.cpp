#include <utility>
#include <memory>

#include "common/SetUtils.h"
#include "ResultGroupFactory.h"

using std::move;

unique_ptr<ResultGroup> ResultGroupFactory::extractResults(
    PQLQueryResult *result, vector<PQLSynonymName> *syns) {
  ResultGroup* resultGroup = new ResultGroup();
  // Add synonyms to the new ResultGroup
  for (const PQLSynonymName& name : *syns) {
    resultGroup->addSynonym(name);
  }

  IntersectSet<int> rowsToTake = getUniqueRows(result, syns);

  for (int rowIdx : rowsToTake) {
    QueryResultTableRow* tableRow = result->getTableRowAt(rowIdx);
    QueryResultTableRow newRow{};
    for (const PQLSynonymName& syn : *syns) {
      ResultTableCol tableCol = result->getSynonymCol(syn);
      newRow.push_back(std::move(tableRow->at(tableCol)));
    }

    resultGroup->addRow(std::move(newRow));
  }
return unique_ptr<ResultGroup>(resultGroup);
}

IntersectSet<int> ResultGroupFactory::getUniqueRows(
    PQLQueryResult *result, vector<PQLSynonymName> *syns) {
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
