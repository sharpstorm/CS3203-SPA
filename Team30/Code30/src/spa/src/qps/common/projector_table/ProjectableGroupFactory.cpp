#include <utility>
#include <memory>

#include "common/SetUtils.h"
#include "ProjectableGroupFactory.h"

using std::move;

ProjectableGroupPtr ProjectableGroupFactory::extractResults(
    PQLQueryResult *result, const PQLSynonymNameList *syns) {
  ProjectableGroupPtr resultGroup = make_unique<ProjectableGroup>();
  // Add synonyms to the new ProjectableGroup
  for (const PQLSynonymName &name : *syns) {
    resultGroup->addSynonym(name);
  }

  IntersectSet<ResultTableRow> rowsToTake = getUniqueRows(result, syns);

  for (const ResultTableRow &rowIdx : rowsToTake) {
    const QueryResultTableRow *tableRow = result->getTableRowAt(rowIdx);
    QueryResultTableRow newRow;
    newRow.reserve(syns->size());

    for (const PQLSynonymName &syn : *syns) {
      ResultTableCol tableCol = result->getSynonymCol(syn);
      newRow.emplace_back(tableRow->at(tableCol));
    }
    resultGroup->addRow(newRow);
  }
  result->releaseOwnedItemsTo(resultGroup->getOwnedPool());
  return resultGroup;
}

IntersectSet<ResultTableRow> ProjectableGroupFactory::getUniqueRows(
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
    const QueryResultTableRow *currRow = result->getTableRowAt(i);

    for (const PQLSynonymName &syn : *syns) {
      ResultTableCol colIdx = result->getSynonymCol(syn);
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
