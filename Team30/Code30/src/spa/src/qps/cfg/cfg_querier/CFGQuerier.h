#pragma once

#include <cassert>

#include "common/Types.h"
#include "ICFGClauseQuerier.h"

template <class T>
class CFGQuerier {
 public:
  StmtTransitiveResult queryArgs(const StmtRef &arg0,
                                 const StmtRef &arg1);
  void queryArgs(const StmtRef &arg0,
                 const StmtRef &arg1,
                 StmtTransitiveResult* existing);
};

template <class T>
StmtTransitiveResult CFGQuerier<T>::queryArgs(const StmtRef &arg0,
                                              const StmtRef &arg1) {
  if (arg0.isKnown() && arg1.isKnown()) {
    return static_cast<T*>(this)->queryBool(arg0.getValue(), arg1.getValue());
  } else if (arg0.isKnown()) {
    return static_cast<T*>(this)->queryFrom(arg0.getValue(), arg1.getType());
  } else if (arg1.isKnown()) {
    return static_cast<T*>(this)->queryTo(arg0.getType(), arg1.getValue());
  }
  assert(false);
  return StmtTransitiveResult();
}

template <class T>
void CFGQuerier<T>::queryArgs(const StmtRef &arg0,
                              const StmtRef &arg1,
                              StmtTransitiveResult* existing) {
  if (arg0.isKnown() || arg1.isKnown()) {
    return;
  }

  static_cast<T*>(this)->queryAll(existing, arg0.getType(), arg1.getType());
}
