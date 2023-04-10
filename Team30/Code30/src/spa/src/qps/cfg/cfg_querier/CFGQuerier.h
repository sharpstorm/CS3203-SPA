#pragma once

#include "common/Types.h"

typedef QueryResult<StmtValue, StmtValue> StmtTransitiveResult;

/*
 * Implements an Abstract class using the Curiously Recurring Template Pattern
 * It enforces that inheriting classes must implement 4 methods:
 *  - queryBool
 *  - queryFrom
 *  - queryTo
 *  - queryAll
 *
 * See: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */

template<class T>
class CFGQuerier {
 public:
  void queryArgs(const StmtRef &arg0,
                 const StmtRef &arg1,
                 StmtTransitiveResult *existing);
};

template<class T>
void CFGQuerier<T>::queryArgs(const StmtRef &arg0,
                              const StmtRef &arg1,
                              StmtTransitiveResult *existing) {
  if (arg0.isKnown() && arg1.isKnown()) {
    static_cast<T *>(this)->queryBool(existing, arg0.getValue(),
                                      arg1.getValue());
  } else if (!arg0.isKnown() && !arg1.isKnown()) {
    static_cast<T *>(this)->queryAll(existing, arg0.getType(),
                                     arg1.getType());
  } else if (arg0.isKnown()) {
    static_cast<T *>(this)->queryFrom(existing, arg0.getValue(),
                                      arg1.getType());
  } else {
    static_cast<T *>(this)->queryTo(existing, arg0.getType(),
                                    arg1.getValue());
  }
}
