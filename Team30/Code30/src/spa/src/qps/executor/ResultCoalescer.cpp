#include "ResultCoalescer.h"
#include <utility>

using std::move, std::make_unique;

PQLQueryResult *ResultCoalescer::merge(PQLQueryResult *setA,
                                       PQLQueryResult *setB) {
  if (setA == nullptr) {
    return setB;
  }

  if (setB == nullptr) {
    return setA;
  }

  PQLQueryResult* result = new PQLQueryResult();
  InternalMergeState internalState{
      setA,
      setB,
      result
  };

  mergeStaticResult(&internalState);
  mergeError(&internalState);

  if (!result->isFalse()) {
    mergeResult(&internalState);
  }

  delete(setA);
  delete(setB);
  return result;
}

void ResultCoalescer::mergeStaticResult(InternalMergeState *state) {
  bool mergedIsFalse = state->setA->isFalse() || state->setB->isFalse();
  state->output->setIsStaticFalse(mergedIsFalse);
}

void ResultCoalescer::mergeError(InternalMergeState *state) {
  string error = "";
  if (!state->setA->getError().empty()) {
    error += state->setA->getError() + " ";
  }

  if (!state->setB->getError().empty()) {
    error += state->setB->getError() + " ";
  }

  state->output->setError(error);
}

void ResultCoalescer::mergeResult(InternalMergeState* mergeState) {
  IntersectState intersectState;
  // init syn list
  mergeSynonymList(mergeState, &intersectState);

  unordered_set<int> ignoreRows;
  for (int i = 0; i < mergeState->setA->getRowCount(); i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }

    auto row = mergeState->setA->getTableRowAt(i);
    IntersectResult intersect = findIntersect(mergeState, row,
                                              &intersectState);
    if (intersect.leftSet == nullptr || intersect.rightSet == nullptr) {
      continue;
    }
    crossProduct(mergeState, &ignoreRows, &intersectState, &intersect);
  }
}

void ResultCoalescer::mergeSynonymList(InternalMergeState *mergeState,
                                       IntersectState *intersectState) {
  auto synonymsA = mergeState->setA->getSynonyms();
  auto synonymsB = mergeState->setB->getSynonyms();
  unordered_set<ResultTableCol> rightColsToIgnore;

  for (auto it = synonymsA->begin(); it != synonymsA->end(); it++) {
    mergeState->output->putSynonym(it->first);
    intersectState->leftColsToCopy.push_back(it->second);
    ResultTableCol rightCol = mergeState->setB->getSynonymCol(it->first);
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
    mergeState->output->putSynonym(it->first);
  }
}

ResultCoalescer::IntersectResult ResultCoalescer::findIntersect(
    InternalMergeState* mergeState,
    QueryResultTableRow* currentRow,
    IntersectState* state) {
  IntersectSetPtr<int> leftSet = nullptr;
  IntersectSetPtr<int> rightSet = nullptr;
  for (int j = 0; j < state->leftCommons.size(); j++) {
    ResultTableCol leftCol = state->leftCommons.at(j);
    ResultTableCol rightCol = state->rightCommons.at(j);
    auto referenceValue = currentRow->at(leftCol).get();
    auto leftSearch = IntersectSetPtr<int>(mergeState->setA
        ->getRowsWithValue(leftCol, referenceValue));
    auto rightSearch = IntersectSetPtr<int>(mergeState->setB
        ->getRowsWithValue(rightCol, referenceValue));
    if (j == 0) {
      leftSet = move(leftSearch);
      rightSet = move(rightSearch);
      continue;
    }

    leftSet = intersectSet(leftSet.get(), leftSearch.get());
    rightSet = intersectSet(rightSet.get(), rightSearch.get());
  }

  return {move(leftSet), move(rightSet)};
}

void ResultCoalescer::crossProduct(InternalMergeState* mergeState,
                                   unordered_set<int>* ignoreSet,
                                   IntersectState* intersectState,
                                   IntersectResult* intersection) {
  for (auto it = intersection->leftSet->begin();
       it != intersection->leftSet->end(); it++) {
    int leftRowNumber = *it;
    ignoreSet->insert(leftRowNumber);
    auto leftRow = mergeState->setA->getTableRowAt(leftRowNumber);

    for (auto it2 = intersection->rightSet->begin();
         it2 != intersection->rightSet->end(); it2++) {
      int rightRowNumber = *it2;
      auto rightRow = mergeState->setB->getTableRowAt(rightRowNumber);
      QueryResultTableRow mergedRow{};
      mergeRow(leftRow, rightRow, &mergedRow, intersectState);
      mergeState->output->putTableRow(move(mergedRow));
    }
  }
}

void ResultCoalescer::mergeRow(QueryResultTableRow *rowA,
                                              QueryResultTableRow *rowB,
                                              QueryResultTableRow *outputRow,
                                              IntersectState *state) {
  for (int j = 0; j < state->leftColsToCopy.size(); j++) {
    ResultTableCol copyCol = state->leftColsToCopy.at(j);
    outputRow->push_back(make_unique<QueryResultItem>(
        *rowA->at(copyCol)));
  }

  for (int j = 0; j < state->rightColsToCopy.size(); j++) {
    ResultTableCol copyCol = state->rightColsToCopy[j];
    outputRow->push_back(make_unique<QueryResultItem>(
        *rowB->at(copyCol)));
  }
}

template<class T>
IntersectSetPtr<T> ResultCoalescer::intersectSet(IntersectSet<T>* s1,
                                                 IntersectSet<T>* s2) {
  if (s1 == nullptr) {
    return nullptr;
  } else if (s2 == nullptr) {
    return nullptr;
  }

  auto result = make_unique<IntersectSet<T>>();
  for (auto it = s1->begin(); it != s1->end(); it++) {
    if (s2->find(*it) != s2->end()) {
      result->insert(*it);
    }
  }

  return result;
}
