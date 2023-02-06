#include "ResultCoalescer.h"

PQLQueryResult *ResultCoalescer::merge(PQLQueryResult *setA,
                                       PQLQueryResult *setB) {
  if (setA == nullptr) {
    return setB;
  } else if (setB == nullptr) {
    return setA;
  }

  PQLQueryResult* result = new PQLQueryResult();
  result->setIsStaticFalse(setA->getIsStaticFalse()
  || setB->getIsStaticFalse());
  string error = "";
  if (setA->getError() != "") {
    error += setA->getError() + " ";
  }
  if (setB->getError() != "") {
    error += setB->getError() + " ";
  }
  result->setError(error);

  for (auto& it : setA->getEntityMap()) {
    result->addToEntityMap(it.first, it.second);
  }

  for (auto& it : setB->getEntityMap()) {
    result->addToEntityMap(it.first, it.second);
  }

  for (auto& it : setA->getStatementMap()) {
    result->addToStatementMap(it.first, it.second);
  }

  for (auto& it : setB->getStatementMap()) {
    result->addToStatementMap(it.first, it.second);
  }

  delete(setA);
  delete(setB);

  return result;
}
