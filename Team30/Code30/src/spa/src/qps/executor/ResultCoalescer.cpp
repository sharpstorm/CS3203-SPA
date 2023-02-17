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

  for (auto it = synonymsA->begin(); it != synonymsA->end(); it++) {
    synList.push_back(it->first);
    ResultTableCol rightCol = setB->getSynonymCol(it->first);
    if (rightCol == PQLQueryResult::NO_COL) {
      continue;
    }

    leftCommons.push_back(it->second);
    rightCommons.push_back(rightCol);
    rightColsToIgnore.insert(it->second);
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
  for (int i = 0; setA->getRowCount(); i++) {
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
      leftSet = intersectSet(leftSet, setA
          ->getRowsWithValue(leftCol, referenceValue));
      rightSet = intersectSet(rightSet, setB
          ->getRowsWithValue(rightCol, referenceValue));
    }

    for (auto it = leftSet->begin(); it != leftSet->end(); it++) {
      int leftRowNumber = *it;
      ignoreRows.insert(leftRowNumber);
      auto leftRow = setA->getTableRowAt(leftRowNumber);

      for (auto it2 = rightSet->begin(); it2 != rightSet->end(); it2++) {
        int rightRowNumber = *it2;
        auto rightRow = setA->getTableRowAt(rightRowNumber);
        QueryResultTableRow mergedRow{};
        for (int j = 0; j < leftRow->size(); j++) {
          mergedRow.push_back(make_unique<QueryResultItem>(*leftRow->at(j)));
        }

        for (int j = 0; j < rightColsToCopy.size(); j++) {
          int copyCol = rightColsToCopy[j];
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
    return s2;
  } else if (s2 == nullptr) {
    return s1;
  }

  auto result = new unordered_set<T>();
  for (auto it = s1->begin(); it != s1->end(); it++) {
    if (s2->find(*it) != s1->end()) {
      result->insert(*it);
    }
  }

  return result;
}
