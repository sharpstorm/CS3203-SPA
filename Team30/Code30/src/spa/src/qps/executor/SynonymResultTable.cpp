#include <memory>
#include <iostream>
#include "common/SetUtils.h"
#include "SynonymResultTable.h"
#include "ResultCoalescer.h"

using std::make_unique, std::unique_ptr;

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping,
                                       bool booleanResult) :
    synonymMapping(mapping), booleanResult(booleanResult) { }

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping) :
    synonymMapping(mapping), booleanResult(false) {}

SynonymResultTable::SynonymResultTable(bool booleanResult) :
    booleanResult(booleanResult), synonymMapping(new PQLQuerySynonymList()) {}

void SynonymResultTable::extractResults(PQLQueryResult *result, vector<PQLSynonymName> syns) {
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
      RowSet* rows = result->getRowsWithValue(colIdx, currRow->at(colIdx).get());
      ignoreRows.insert(rows->begin(), rows->end());
    }

    if (i==0) {
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

//ResultGroup SynonymResultTable::getResultGroup(int idx) {
//  return groupResults[idx];
//}

int SynonymResultTable::getResultGroupCount() {
  return groupResults.size();
}
ResultGroup* SynonymResultTable::getResultGroup(int idx) {
//  unique_ptr<ResultGroup> rGroup = unique_ptr<ResultGroup>(groupResults.at(idx).get());
  return groupResults.at(idx).get();
}
