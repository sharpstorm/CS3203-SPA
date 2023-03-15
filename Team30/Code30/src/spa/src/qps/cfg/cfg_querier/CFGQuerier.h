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
    return static_cast<T*>(this)->queryBool(arg0.lineNum, arg1.lineNum);
  } else if (arg0.isKnown()) {
    return static_cast<T*>(this)->queryFrom(arg0.lineNum, arg1.type);
  } else if (arg1.isKnown()) {
    return static_cast<T*>(this)->queryTo(arg0.type, arg1.lineNum);
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

  static_cast<T*>(this)->queryAll(existing, arg0.type, arg1.type);
}
