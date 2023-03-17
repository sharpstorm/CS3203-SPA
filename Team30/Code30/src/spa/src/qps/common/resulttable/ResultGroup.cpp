#include <utility>
#include <memory>
#include "ResultGroup.h"

using std::make_unique;

void ResultGroup::addRow(QueryResultTableRow row) {
  groupTable.push_back(std::move(row));
}

void ResultGroup::addSynonym(PQLSynonymName name) {
  int curIndex = colIdx.size();
  colMap.emplace(name, curIndex);
  colIdx.push_back(name);
}

void ResultGroup::addColMap(vector<PQLSynonymName> map) {
  for (const PQLSynonymName& syn : map) {
    addSynonym(syn);
  }
}

vector<PQLSynonymName>* ResultGroup::getColIndexes() {
  return &colIdx;
}

int ResultGroup::getTableRows() {
  return groupTable.size();
}

QueryResultTableRow* ResultGroup::getRowAt(int idx) {
  return &groupTable.at(idx);
}

ResultGroup* ResultGroup::crossProduct(ResultGroup* other) {
  ResultGroup* output = new ResultGroup();
  output->addColMap(colIdx);
  output->addColMap(*other->getColIndexes());

  for (int i=0; i < getTableRows(); i++) {
    QueryResultTableRow* leftRow = getRowAt(i);
    for (int j=0; j < other->getTableRows(); j++) {
      QueryResultTableRow* rightRow = other->getRowAt(j);

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

void ResultGroup::project(AttributedSynonymList *synList,
                            PkbQueryHandler* handler,
                            vector<string>* result) {
  // Iterate through each row
  for (int i=0; i < getTableRows(); i++) {
    QueryResultTableRow* row = getRowAt(i);
    string rowString;
    for (int j=0; j < synList->size(); j++) {
      AttributedSynonym syn = synList->at(j);
      ResultTableCol col = colMap.at(syn.getName());
      QueryResultItem* queryItem = row->at(col).get();

      bool synIsStmtType = syn.isStatementType();
      bool synOutputsInteger = syn.returnsInteger();
      bool isTypeConstant = syn.getType() == PQL_SYN_TYPE_CONSTANT;
      if (syn.hasAttribute() && !isTypeConstant && synIsStmtType != synOutputsInteger) {
        string attrVal;
        // read/print.varName
        if (synIsStmtType) {
          if (syn.getType() == PQL_SYN_TYPE_READ) {
           attrVal = handler->getReadDeclarations(queryItem->getStmtRef());
          } else {
            attrVal = handler->getPrintDeclarations(queryItem->getStmtRef());
          }
        } else {
          // call.procName
          attrVal = handler->getCalledDeclaration(queryItem->getStmtRef());
        }
        rowString += attrVal;
      } else {
        rowString += queryItem->project();
      }

      if (j < synList->size() - 1) {
        rowString += " ";
      }
    }
    result->push_back(rowString);
  }
}

bool ResultGroup::operator==(const ResultGroup &rg) const {
  if (colMap.size() != rg.colMap.size() ||
      groupTable.size() != groupTable.size()) {
    return false;
  }

  for (const auto& it : colMap) {
    if (rg.colMap.find(it.first) == rg.colMap.end()) {
      return false;
    }
  }

  for (int i= 0; i < groupTable.size(); i++) {
    bool isFound = false;
    for (int j = 0; j < rg.groupTable.size(); j++) {
      // Differing column lengths
      if (groupTable[i].size() != rg.groupTable[j].size()) {
        return false;
      }

      // Go through all the synonyms
      for (auto it : rg.colMap) {
        int otherIdx = it.second;
        int thisIdx = colMap.at(it.first);

        if (*groupTable[i][thisIdx].get() ==
            *rg.groupTable[j][otherIdx].get()) {
          isFound = true;
          break;
        }
      }
      if (isFound) {
        break;
      }
    }

    if (!isFound) {
      return false;
    }
  }

  return true;
}
