#include <utility>
#include <memory>
#include <string>

#include "ProjectorResultGroup.h"

using std::make_unique, std::to_string;

void ProjectorResultGroup::addRow(QueryResultTableRow row) {
  groupTable.push_back(std::move(row));
}

void ProjectorResultGroup::addSynonym(PQLSynonymName name) {
  int curIndex = colIdx.size();
  colMap.emplace(name, curIndex);
  colIdx.push_back(name);
}

void ProjectorResultGroup::addColMap(vector<PQLSynonymName> map) {
  for (const PQLSynonymName& syn : map) {
    addSynonym(syn);
  }
}

vector<PQLSynonymName>* ProjectorResultGroup::getColIndexes() {
  return &colIdx;
}

int ProjectorResultGroup::getTableRows() {
  return groupTable.size();
}

QueryResultTableRow* ProjectorResultGroup::getRowAt(int idx) {
  return &groupTable.at(idx);
}

ProjectorResultGroup* ProjectorResultGroup::crossProduct(ProjectorResultGroup* other) {
  ProjectorResultGroup* output = new ProjectorResultGroup();
  output->addColMap(colIdx);
  output->addColMap(*other->getColIndexes());

  for (int i=0; i < getTableRows(); i++) {
    QueryResultTableRow* leftRow = getRowAt(i);
    for (int j=0; j < other->getTableRows(); j++) {
      QueryResultTableRow* rightRow = other->getRowAt(j);

      QueryResultTableRow rowToAdd{};
      for (const auto & k : *leftRow) {
        rowToAdd.push_back(k);
      }

      for (const auto & k : *rightRow) {
        rowToAdd.push_back(k);
      }

      output->addRow(std::move(rowToAdd));
    }
  }

  return output;
}

void ProjectorResultGroup::project(AttributedSynonymList *synList,
                                   PkbQueryHandler* handler,
                                   vector<string>* result) {
  // Iterate through each row
  for (int i=0; i < getTableRows(); i++) {
    QueryResultTableRow* row = getRowAt(i);
    string rowString;

    for (int j=0; j < synList->size(); j++) {
      AttributedSynonym syn = synList->at(j);
      ResultTableCol col = colMap.at(syn.getName());
      QueryResultItem* queryItem = row->at(col);

      // Special case attribution read/print.varName and call.procName
      if (isNonDefaultCase(syn)) {
        rowString += projectNonDefaultAttribute(handler, queryItem, syn);
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

bool ProjectorResultGroup::operator==(const ProjectorResultGroup &rg) const {
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

        if (*groupTable[i][thisIdx] == *rg.groupTable[j][otherIdx]) {
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

string ProjectorResultGroup::projectNonDefaultAttribute(PkbQueryHandler *handler,
                                                        QueryResultItem *item,
                                                        AttributedSynonym syn) {
  if (syn.getType() == PQL_SYN_TYPE_READ) {
    // read.varName
    return handler->getReadDeclarations(item->toStmtValue());
  } else if (syn.getType() == PQL_SYN_TYPE_PRINT) {
    // print.varName
    return  handler->getPrintDeclarations(item->toStmtValue());
  }

  // call.procName
  return handler->getCalledDeclaration(item->toStmtValue());
}

bool ProjectorResultGroup::isNonDefaultCase(AttributedSynonym syn) {
  bool isTypeConstant = syn.getType() == PQL_SYN_TYPE_CONSTANT;
  return syn.hasAttribute() && !isTypeConstant &&
          syn.isStatementType() != syn.returnsInteger();
}

QueryResultItemPool *ProjectorResultGroup::getOwnedPool() {
  return &ownedItems;
}
