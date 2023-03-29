#include <utility>
#include <unordered_set>

#include "ResultCoalescer.h"
#include "common/SetUtils.h"

using std::make_unique, std::unordered_set;

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
    mergeResults();
  }

  delete setA;
  delete setB;
  return output;
}

void ResultCoalescer::mergeResults() {
  IntersectState intersectState;
  // init syn list
  mergeSynonymList(&intersectState);
  output->adoptOwnedItems(setA);
  orphanMap = output->adoptOwnedItems(setB);

  set<ResultTableRow> ignoreRows;
  for (int i = 0; i < setA->getRowCount(); i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }

    auto row = setA->getTableRowAt(i);
    IntersectResult intersect = findIntersect(row, &intersectState);
    if (intersect.leftSet == nullptr || intersect.rightSet == nullptr) {
      continue;
    }
    crossProduct(&ignoreRows, &intersectState, &intersect);
  }
}

void ResultCoalescer::mergeSynonymList(IntersectState *intersectState) {
  auto synonymsA = setA->getSynonyms();
  auto synonymsB = setB->getSynonyms();
  unordered_set<ResultTableCol> rightColsToIgnore;

  for (auto it = synonymsA->begin(); it != synonymsA->end(); it++) {
    output->putSynonym(it->first);
    intersectState->leftColsToCopy.push_back(it->second);
    ResultTableCol rightCol = setB->getSynonymCol(it->first);
    if (rightCol == PQLQueryResult::NO_COL) {
      continue;
    }

    intersectState->leftCommons.push_back(it->second);
    intersectState->rightCommons.push_back(rightCol);
    rightColsToIgnore.insert(rightCol);
  }

  if (intersectState->leftCommons.size() == 0) {
    return;
  }

  for (auto it = synonymsB->begin(); it != synonymsB->end(); it++) {
    if (rightColsToIgnore.find(it->second) != rightColsToIgnore.end()) {
      continue;
    }

    intersectState->rightColsToCopy.push_back(it->second);
    output->putSynonym(it->first);
  }
}

ResultCoalescer::IntersectResult ResultCoalescer::findIntersect(
    QueryResultTableRow *currentRow,
    IntersectState *state) {
  IntersectSetPtr<ResultTableRow> leftSet = nullptr;
  IntersectSetPtr<ResultTableRow> rightSet = nullptr;
  for (int j = 0; j < state->leftCommons.size(); j++) {
    ResultTableCol leftCol = state->leftCommons.at(j);
    ResultTableCol rightCol = state->rightCommons.at(j);
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

    if (rightSet == nullptr || rightSet == nullptr) {
      break;
    }
  }

  return {std::move(leftSet), std::move(rightSet)};
}

void ResultCoalescer::crossProduct(set<ResultTableRow> *ignoreSet,
                                   IntersectState *intersectState,
                                   IntersectResult *intersection) {
  if (intersection->isEmpty()) {
    return;
  }

  for (auto it = intersection->leftSet->begin();
       it != intersection->leftSet->end(); it++) {
    ResultTableRow leftRowNumber = *it;
    ignoreSet->insert(leftRowNumber);
    auto leftRow = setA->getTableRowAt(leftRowNumber);

    for (auto it2 = intersection->rightSet->begin();
         it2 != intersection->rightSet->end(); it2++) {
      ResultTableRow rightRowNumber = *it2;
      auto rightRow = setB->getTableRowAt(rightRowNumber);
      QueryResultTableRow mergedRow{};
      mergeRow(leftRow, rightRow, &mergedRow, intersectState);
      output->putTableRow(mergedRow);
    }
  }
}

void ResultCoalescer::mergeRow(QueryResultTableRow *rowA,
                               QueryResultTableRow *rowB,
                               QueryResultTableRow *outputRow,
                               IntersectState *state) {
  for (int j = 0; j < state->leftColsToCopy.size(); j++) {
    ResultTableCol copyCol = state->leftColsToCopy.at(j);
    outputRow->push_back(orphanMap->getMappingFor(rowA->at(copyCol)));
  }

  for (int j = 0; j < state->rightColsToCopy.size(); j++) {
    ResultTableCol copyCol = state->rightColsToCopy[j];
    outputRow->push_back(orphanMap->getMappingFor(rowB->at(copyCol)));
  }
}
