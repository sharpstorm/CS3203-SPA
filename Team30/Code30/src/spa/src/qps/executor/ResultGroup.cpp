#include <memory>
#include "ResultGroup.h"

using std::make_unique;

void ResultGroup::addRow(QueryResultTableRow row) {
  groupTable.push_back(std::move(row));
}

void ResultGroup::addSynonym(PQLSynonymName name) {
  int idx = colIdx.size();
    colIdx.push_back(name);
    colMap.insert({name, idx});
}

void ResultGroup::addColMap(vector<PQLSynonymName> map) {
  for (const PQLSynonymName& syn: map) {
    addSynonym(syn);
  }
}

vector<PQLSynonymName> ResultGroup::getColIndexes() {
  return colIdx;
}

int ResultGroup::getTableRows() {
  return groupTable.size();
}

QueryResultTableRow* ResultGroup::getQueryItemAt(int idx) {
  return &groupTable.at(idx);
}

ResultGroup* ResultGroup::crossProduct(ResultGroup* other) {
  ResultGroup* output = new ResultGroup();
  // !! Column ordering is messed up here !!
  output->addColMap(colIdx);
  output->addColMap(other->getColIndexes());

  // For each left Row
  // For each right Row
  for (int i=0; i < getTableRows(); i++) {
    QueryResultTableRow* leftRow = getQueryItemAt(i);
    for (int j=0; j < other->getTableRows(); j++) {
      QueryResultTableRow* rightRow = other->getQueryItemAt(j);

      QueryResultTableRow rowToAdd{};
      for (const auto & k : *leftRow) {
        rowToAdd.push_back(make_unique<QueryResultItem>(*k));
      }

      for (const auto & k : *rightRow) {
        rowToAdd.push_back(make_unique<QueryResultItem>(*k));
      }

      output->addRow(std::move(rowToAdd));
    }
  }

  return output;
}

void ResultGroup::project(PQLQuerySynonymList *synList,
                            vector<string>* result) {
  // Iterate through each row
  for (int i=0; i < getTableRows(); i++) {
   QueryResultTableRow* row = getQueryItemAt(i);
   string rowString;
   for (int j=0; j < synList->size(); j++) {
     // Get the column index from the result group
     PQLQuerySynonym syn = synList->at(j);
     ResultTableCol col = colMap.at(syn.getName());
     rowString += row->at(col)->project();
     if (j < synList->size() - 1) {
       rowString += " ";
     }
   }
  result->push_back(rowString);
  }
}
