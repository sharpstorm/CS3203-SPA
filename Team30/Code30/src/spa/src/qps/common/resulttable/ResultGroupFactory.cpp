#include <utility>
#include <memory>

#include "common/SetUtils.h"
#include "ResultGroupFactory.h"

using std::move;

ResultGroupPtr ResultGroupFactory::extractResults(
    PQLQueryResult *result, vector<PQLSynonymName> *syns) {
  ResultGroupPtr resultGroup = make_unique<ResultGroup>();
  // Add synonyms to the new ResultGroup
  for (const PQLSynonymName& name : *syns) {
    resultGroup->addSynonym(name);
  }

  IntersectSet<ResultTableRow> rowsToTake = getUniqueRows(result, syns);

  for (ResultTableRow rowIdx : rowsToTake) {
    QueryResultTableRow* tableRow = result->getTableRowAt(rowIdx);
    QueryResultTableRow newRow{};
    for (const PQLSynonymName& syn : *syns) {
      ResultTableCol tableCol = result->getSynonymCol(syn);
      newRow.push_back(tableRow->at(tableCol));
    }
    resultGroup->addRow(newRow);
  }
  result->releaseOwnedItemsTo(resultGroup->getOwnedPool());
  return resultGroup;
}

IntersectSet<ResultTableRow> ResultGroupFactory::getUniqueRows(
    PQLQueryResult *result, vector<PQLSynonymName> *syns) {
  IntersectSet<ResultTableRow> rowsToTake;
  IntersectSet<ResultTableRow> ignoreRows;
  int rowCounts = result->getRowCount();
  for (int i = 0; i < rowCounts; i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }
    IntersectSetPtr<ResultTableRow> currentIgnoreRows =
        make_unique<IntersectSet<ResultTableRow>>();

    rowsToTake.insert(i);
    for (const PQLSynonymName& syn : *syns) {
      ResultTableCol colIdx = result->getSynonymCol(syn);
      QueryResultTableRow* currRow = result->getTableRowAt(i);
      RowSetPtr rows = result->getRowsWithValue(colIdx,
                                                currRow->at(colIdx));
      if (currentIgnoreRows->empty()) {
        currentIgnoreRows->insert(rows->begin(), rows->end());
      } else {
        currentIgnoreRows = SetUtils::intersectSet(currentIgnoreRows.get(),
                                                   rows.get());
      }
    }

    ignoreRows.insert(currentIgnoreRows->begin(), currentIgnoreRows->end());
  }

  return rowsToTake;
}
