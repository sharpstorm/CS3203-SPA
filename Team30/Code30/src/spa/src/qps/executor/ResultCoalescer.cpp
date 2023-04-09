#include <utility>
#include <unordered_set>

#include "ResultCoalescer.h"
#include "common/SetUtils.h"

using std::unordered_set;

ResultCoalescer::ResultCoalescer(PQLQueryResult *setA,
                                 PQLQueryResult *setB) :
    setA(setA), setB(setB) {}

PQLQueryResult *ResultCoalescer::merge() {
  if (setA == nullptr) {
    return setB;
  }

  if (setB == nullptr) {
    return setA;
  }

  output = new PQLQueryResult();
  if (!setA->isFalse() && !setB->isFalse()) {
    // Optimization: Set A should be smaller set
    if (setB->getRowCount() < setA->getRowCount()) {
      swapSets();
    }
    mergeResults();
  }

  delete setA;
  delete setB;
  return output;
}

void ResultCoalescer::mergeResults() {
  // init syn list
  mergeSynonymList();
  output->adoptOwnedItems(setA);
  orphanMap = output->adoptOwnedItems(setB);

  set<ResultTableRow> ignoreRows;
  for (int i = 0; i < setA->getRowCount(); i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }

    auto row = setA->getTableRowAt(i);
    IntersectResult intersect = findIntersect(row);
    if (intersect.leftSet == nullptr || intersect.rightSet == nullptr) {
      continue;
    }
    crossProduct(&ignoreRows, &intersect);
  }
}

void ResultCoalescer::mergeSynonymList() {
  auto synonymsA = setA->getSynonyms();
  auto synonymsB = setB->getSynonyms();
  set<ResultTableCol> rightColsToIgnore;

  for (auto it = synonymsA->begin(); it != synonymsA->end(); it++) {
    output->putSynonym(it->first);
    leftColsToCopy.push_back(it->second);
    ResultTableCol rightCol = setB->getSynonymCol(it->first);
    if (rightCol == PQLQueryResult::NO_COL) {
      continue;
    }

    leftCommons.push_back(it->second);
    rightCommons.push_back(rightCol);
    rightColsToIgnore.insert(rightCol);
  }

  if (leftCommons.empty()) {
    return;
  }

  for (auto it = synonymsB->begin(); it != synonymsB->end(); it++) {
    if (rightColsToIgnore.find(it->second) != rightColsToIgnore.end()) {
      continue;
    }

    rightColsToCopy.push_back(it->second);
    output->putSynonym(it->first);
  }
}

ResultCoalescer::IntersectResult ResultCoalescer::findIntersect(
    const QueryResultTableRow *currentRow) const {
  RowSetPtr leftSet = nullptr;
  RowSetPtr rightSet = nullptr;
  for (size_t j = 0; j < leftCommons.size(); j++) {
    ResultTableCol leftCol = leftCommons.at(j);
    ResultTableCol rightCol = rightCommons.at(j);
    auto referenceValue = currentRow->at(leftCol);
    auto leftSearch = setA->getRowsWithValue(leftCol, referenceValue);
    auto rightSearch = setB->getRowsWithValue(rightCol, referenceValue);
    if (j == 0) {
      leftSet = std::move(leftSearch);
      rightSet = std::move(rightSearch);
      continue;
    }

    leftSet = SetUtils::intersectSet(leftSet.get(), leftSearch.get());
    rightSet = SetUtils::intersectSet(rightSet.get(), rightSearch.get());

    if (leftSet == nullptr || rightSet == nullptr) {
      break;
    }
  }

  return {std::move(leftSet), std::move(rightSet)};
}

void ResultCoalescer::crossProduct(set<ResultTableRow> *ignoreSet,
                                   const IntersectResult *intersection) {
  if (intersection->isEmpty()) {
    return;
  }

  const RowSetPtr &leftSet = intersection->leftSet;
  const RowSetPtr &rightSet = intersection->rightSet;

  for (auto it = leftSet->begin(); it != leftSet->end(); it++) {
    ResultTableRow leftRowNumber = *it;
    ignoreSet->insert(leftRowNumber);
    auto leftRow = setA->getTableRowAt(leftRowNumber);

    for (auto it2 = rightSet->begin(); it2 != rightSet->end(); it2++) {
      ResultTableRow rightRowNumber = *it2;
      auto rightRow = setB->getTableRowAt(rightRowNumber);
      QueryResultTableRow mergedRow;
      mergedRow.reserve(leftColsToCopy.size() + rightColsToCopy.size());
      mergeRow(leftRow, rightRow, &mergedRow);
      output->putTableRow(mergedRow);
    }
  }
}

void ResultCoalescer::mergeRow(const QueryResultTableRow *rowA,
                               const QueryResultTableRow *rowB,
                               QueryResultTableRow *outputRow) const {
  for (int j = 0; j < leftColsToCopy.size(); j++) {
    ResultTableCol copyCol = leftColsToCopy.at(j);
    outputRow->push_back(orphanMap->getMappingFor(rowA->at(copyCol)));
  }

  for (int j = 0; j < rightColsToCopy.size(); j++) {
    ResultTableCol copyCol = rightColsToCopy[j];
    outputRow->push_back(orphanMap->getMappingFor(rowB->at(copyCol)));
  }
}

bool ResultCoalescer::IntersectResult::isEmpty() const {
  return leftSet->empty() || rightSet->empty();
}

void ResultCoalescer::swapSets() {
  PQLQueryResult *temp = setA;
  setA = setB;
  setB = temp;
}
