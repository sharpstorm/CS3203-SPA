#include "ResultCoalescer.h"

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
  mergeEntityResult(setA, setB, result);
  mergeStatementResult(setA, setB, result);

  delete(setA);
  delete(setB);
  return result;
}

bool ResultCoalescer::mergeStaticResult(PQLQueryResult *setA,
                                        PQLQueryResult *setB) {
  return setA->getIsStaticFalse() || setB->getIsStaticFalse();
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

void ResultCoalescer::mergeEntityResult(PQLQueryResult *setA,
                                        PQLQueryResult *setB,
                                        PQLQueryResult *output) {
  for (auto& it : setA->getEntityMap()) {
    output->addToEntityMap(it.first, it.second);
  }

  for (auto& it : setB->getEntityMap()) {
    output->addToEntityMap(it.first, it.second);
  }
}

void ResultCoalescer::mergeStatementResult(PQLQueryResult *setA,
                                           PQLQueryResult *setB,
                                           PQLQueryResult *output) {
  for (auto& it : setA->getStatementMap()) {
    output->addToStatementMap(it.first, it.second);
  }

  for (auto& it : setB->getStatementMap()) {
    output->addToStatementMap(it.first, it.second);
  }
}

