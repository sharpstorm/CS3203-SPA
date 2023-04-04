#include <utility>
#include <memory>

#include "common/SetUtils.h"
#include "ProjectorResultFactory.h"

using std::move;

ResultGroupPtr ProjectorResultFactory::extractResults(
    PQLQueryResult *result, const PQLSynonymNameList *syns) {
  ResultGroupPtr resultGroup = make_unique<ProjectorResultGroup>();
  // Add synonyms to the new ProjectorResultGroup
  for (const PQLSynonymName &name : *syns) {
    resultGroup->addSynonym(name);
  }

  IntersectSet<ResultTableRow> rowsToTake = getUniqueRows(result, syns);

  for (const ResultTableRow &rowIdx : rowsToTake) {
    const QueryResultTableRow *tableRow = result->getTableRowAt(rowIdx);
    QueryResultTableRow newRow{};
    for (const PQLSynonymName &syn : *syns) {
      ResultTableCol tableCol = result->getSynonymCol(syn);
      newRow.push_back(tableRow->at(tableCol));
    }
    resultGroup->addRow(newRow);
  }
  result->releaseOwnedItemsTo(resultGroup->getOwnedPool());
  return resultGroup;
}

IntersectSet<ResultTableRow> ProjectorResultFactory::getUniqueRows(
    PQLQueryResult *result, const PQLSynonymNameList *syns) {
  IntersectSet<ResultTableRow> rowsToTake;
  IntersectSet<ResultTableRow> ignoreRows;
  int rowCounts = result->getRowCount();
  for (int i = 0; i < rowCounts; i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }
    RowSetPtr currentIgnoreRows = make_unique<RowSet>();

    rowsToTake.insert(i);
    for (const PQLSynonymName &syn : *syns) {
      ResultTableCol colIdx = result->getSynonymCol(syn);
      const QueryResultTableRow *currRow = result->getTableRowAt(i);
      RowSetPtr rows = result->getRowsWithValue(colIdx,
                                                currRow->at(colIdx));
      if (currentIgnoreRows->empty()) {
        currentIgnoreRows = std::move(rows);
      } else {
        currentIgnoreRows = SetUtils::intersectSet(currentIgnoreRows.get(),
                                                   rows.get());
      }
    }

    ignoreRows.insert(currentIgnoreRows->begin(), currentIgnoreRows->end());
  }

  return rowsToTake;
}
