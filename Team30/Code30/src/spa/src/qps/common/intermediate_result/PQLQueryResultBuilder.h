#pragma once

#include "PQLQueryResult.h"
#include "qps/clauses/arguments/ClauseArgument.h"

template<class T, class U>
class PQLQueryResultBuilder {
 private:
  static PQLSynonymName DEFAULT_REF;
  PQLQueryResultPtr inProgress;

  bool isLeftKnown;
  bool isRightKnown;
  T leftValue;
  U rightValue;

  bool isLeftNamed;
  bool isRightNamed;
  PQLSynonymName leftName;
  PQLSynonymName rightName;

  void populateTwoNamed(const QueryResult<T, U> *queryResult);
  void populateLeftNamed(const QueryResultSet<T> &queryResult);
  void populateRightNamed(const QueryResultSet<U> &queryResult);

 public:
  PQLQueryResultBuilder();
  void setLeftName(ClauseArgument *arg);
  void setRightName(ClauseArgument *arg);
  void setLeftName(const PQLSynonymName &name);
  void setRightName(const PQLSynonymName &name);

  template<class V>
  void setLeftRef(const IRef<T, V> &ref);
  template<class V>
  void setRightRef(const IRef<U, V> &ref);

  PQLQueryResult *build(const QueryResult<T, U> *queryResult);
  PQLQueryResult *build(const QueryResultSet<T> &queryResult);
};

template<class T, class U>
PQLQueryResultBuilder<T, U>::PQLQueryResultBuilder() :
    inProgress(make_unique<PQLQueryResult>()),
    isLeftKnown(false), isRightKnown(false),
    isLeftNamed(false), isRightNamed(false),
    leftName(DEFAULT_REF), rightName(DEFAULT_REF) {}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::setLeftName(ClauseArgument *arg) {
  if (arg->isNamed()) {
    setLeftName(arg->getName());
  }
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::setLeftName(const PQLSynonymName &name) {
  isLeftNamed = true;
  leftName = name;
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::setRightName(ClauseArgument *arg) {
  if (arg->isNamed()) {
    setRightName(arg->getName());
  }
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::setRightName(const PQLSynonymName &name) {
  isRightNamed = true;
  rightName = name;
}

template<class T, class U>
template<class V>
void PQLQueryResultBuilder<T, U>::setLeftRef(const IRef<T, V> &ref) {
  isLeftKnown = ref.isKnown();
  leftValue = ref.getValue();
}

template<class T, class U>
template<class V>
void PQLQueryResultBuilder<T, U>::setRightRef(const IRef<U, V> &ref) {
  isRightKnown = ref.isKnown();
  rightValue = ref.getValue();
}

template<class T, class U>
PQLQueryResult *PQLQueryResultBuilder<T, U>::build(
    const QueryResult<T, U> *queryResult) {
  if (!isLeftNamed && !isRightNamed) {
    inProgress->setIsStaticFalse(queryResult->isEmpty);
  } else if (isLeftNamed && isRightNamed) {
    populateTwoNamed(queryResult);
  } else if (isLeftNamed) {
    populateLeftNamed(queryResult->firstArgVals);
  } else if (isRightNamed) {
    populateRightNamed(queryResult->secondArgVals);
  }

  PQLQueryResult *result = inProgress.get();
  inProgress.release();
  return result;
}

template<class T, class U>
PQLQueryResult *PQLQueryResultBuilder<T, U>::build(
    const QueryResultSet<T> &queryResult) {
  if (!isLeftNamed) {
    inProgress->setIsStaticFalse(queryResult.empty());
  } else {
    populateLeftNamed(queryResult);
  }

  PQLQueryResult *result = inProgress.get();
  inProgress.release();
  return result;
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::populateTwoNamed(
    const QueryResult<T, U> *queryResult) {
  if (isLeftKnown && isRightKnown && !queryResult->isEmpty) {
    pair_set<T, U> dummySet;
    dummySet.insert(pair<T, U>{leftValue, rightValue});
    inProgress->add(leftName, rightName, dummySet);
  } else if (isLeftKnown) {
    inProgress->addLeftConst(leftName, rightName,
                             leftValue, queryResult->secondArgVals);
  } else if (isRightKnown) {
    inProgress->addRightConst(leftName, rightName,
                              queryResult->firstArgVals, rightValue);
  } else {
    inProgress->add(leftName, rightName, queryResult->pairVals);
  }
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::populateLeftNamed(
    const QueryResultSet<T> &result) {
  if (isLeftKnown && !result.empty()) {
    inProgress->add(leftName, QueryResultSet<T>{leftValue});
    return;
  }
  inProgress->add(leftName, result);
}

template<class T, class U>
void PQLQueryResultBuilder<T, U>::populateRightNamed(
    const QueryResultSet<U> &result) {
  if (isRightKnown && !result.empty()) {
    inProgress->add(rightName, QueryResultSet<U>{rightValue});
    return;
  }
  inProgress->add(rightName, result);
}

template<class T, class U>
PQLSynonymName PQLQueryResultBuilder<T, U>::DEFAULT_REF = "";
