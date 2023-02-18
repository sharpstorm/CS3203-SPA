#include "ResultCoalescer.h"
#include <memory>
#include <utility>
#include <vector>

using std::move, std::make_unique, std::vector;

PQLQueryResult *ResultCoalescer::merge(PQLQueryResult *setA,
                                       PQLQueryResult *setB) {
  if (setA == nullptr) {
    return setB;
  }

  if (setB == nullptr) {
    return setA;
  }

  PQLQueryResult* result = new PQLQueryResult();
  result->setIsStaticFalse(mergeStaticResult(setA, setB));
  result->setError(mergeError(setA, setB));

  if (!result->isFalse()) {
    mergeResult(setA, setB, result);
  }

  delete(setA);
  delete(setB);
  return result;
}

bool ResultCoalescer::mergeStaticResult(PQLQueryResult *setA,
                                        PQLQueryResult *setB) {
  return setA->isFalse() || setB->isFalse();
}

string ResultCoalescer::mergeError(PQLQueryResult *setA,
                                   PQLQueryResult *setB) {
  string error = "";
  if (!setA->getError().empty()) {
    error += setA->getError() + " ";
  }

  if (!setB->getError().empty()) {
    error += setB->getError() + " ";
  }

  return error;
}

void ResultCoalescer::mergeResult(PQLQueryResult *setA,
                                  PQLQueryResult *setB,
                                  PQLQueryResult *output) {
  // init syn list
  auto synonymsA = setA->getSynonyms();
  auto synonymsB = setB->getSynonyms();
  vector<ResultTableCol> leftCommons;
  vector<ResultTableCol> rightCommons;
  vector<PQLSynonymName> synList;
  unordered_set<ResultTableCol> rightColsToIgnore;
  vector<ResultTableCol> rightColsToCopy;
  vector<ResultTableCol> leftColsToCopy;

  for (auto it = synonymsA->begin(); it != synonymsA->end(); it++) {
    synList.push_back(it->first);
    leftColsToCopy.push_back(it->second);
    ResultTableCol rightCol = setB->getSynonymCol(it->first);
    if (rightCol == PQLQueryResult::NO_COL) {
      continue;
    }

    leftCommons.push_back(it->second);
    rightCommons.push_back(rightCol);
    rightColsToIgnore.insert(rightCol);
  }

  if (leftCommons.size() == 0) {
    return;
  }

  for (auto it = synonymsB->begin(); it != synonymsB->end(); it++) {
    if (rightColsToIgnore.find(it->second) != rightColsToIgnore.end()) {
      continue;
    }

    rightColsToCopy.push_back(it->second);
    synList.push_back(it->first);
  }

  for (string syn : synList) {
    output->putSynonym(syn);
  }

  unordered_set<int> ignoreRows;
  for (int i = 0; i < setA->getRowCount(); i++) {
    if (ignoreRows.find(i) != ignoreRows.end()) {
      continue;
    }

    auto row = setA->getTableRowAt(i);
    unordered_set<int>* leftSet = nullptr;
    unordered_set<int>* rightSet = nullptr;
    for (int j = 0; j < leftCommons.size(); j++) {
      ResultTableCol leftCol = leftCommons.at(j);
      ResultTableCol rightCol = rightCommons.at(j);
      auto referenceValue = row->at(leftCol).get();
      auto x = setA
          ->getRowsWithValue(leftCol, referenceValue);
      auto y = setB
          ->getRowsWithValue(rightCol, referenceValue);
      if (j == 0) {
        leftSet = x;
        rightSet = y;
      } else {
        leftSet = intersectSet(leftSet, x);
        rightSet = intersectSet(rightSet, y);
      }
    }

    if (leftSet == nullptr || rightSet == nullptr) {
      continue;
    }

    for (auto it = leftSet->begin(); it != leftSet->end(); it++) {
      int leftRowNumber = *it;
      ignoreRows.insert(leftRowNumber);
      auto leftRow = setA->getTableRowAt(leftRowNumber);

      for (auto it2 = rightSet->begin(); it2 != rightSet->end(); it2++) {
        int rightRowNumber = *it2;
        auto rightRow = setB->getTableRowAt(rightRowNumber);
        QueryResultTableRow mergedRow{};
        for (int j = 0; j < leftColsToCopy.size(); j++) {
          ResultTableCol copyCol = leftColsToCopy.at(j);
          mergedRow.push_back(make_unique<QueryResultItem>(
              *leftRow->at(copyCol)));
        }

        for (int j = 0; j < rightColsToCopy.size(); j++) {
          ResultTableCol copyCol = rightColsToCopy[j];
          mergedRow.push_back(make_unique<QueryResultItem>(
              *rightRow->at(copyCol)));
        }

        output->putTableRow(move(mergedRow));
      }
    }
  }
}

template<class T>
unordered_set<T>* ResultCoalescer::intersectSet(unordered_set<T> *s1,
                                                unordered_set<T> *s2) {
  if (s1 == nullptr) {
    return nullptr;
  } else if (s2 == nullptr) {
    return nullptr;
  }

  auto result = new unordered_set<T>();
  for (auto it = s1->begin(); it != s1->end(); it++) {
    if (s2->find(*it) != s2->end()) {
      result->insert(*it);
    }
  }

  return result;
}
