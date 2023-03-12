#pragma once

#include <memory>

#include "common/Types.h"

using std::unique_ptr;

typedef QueryResult<StmtValue, StmtValue> StmtTransitiveResult;

class ICFGQuerier {
 public:
  virtual StmtTransitiveResult queryBool(const StmtValue &arg0,
                                         const StmtValue &arg1) = 0;
  virtual StmtTransitiveResult queryFrom(const StmtValue &arg0) = 0;
  virtual StmtTransitiveResult queryTo(const StmtValue &arg1) = 0;
  virtual StmtTransitiveResult queryAll() = 0;
};

typedef unique_ptr<ICFGQuerier> ICFGQuerierPtr;
