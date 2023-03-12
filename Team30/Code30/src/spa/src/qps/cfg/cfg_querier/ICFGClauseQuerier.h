#pragma once

#include <memory>

#include "common/Types.h"

using std::unique_ptr;

typedef QueryResult<StmtValue, StmtValue> StmtTransitiveResult;

class ICFGClauseQuerier {
 public:
  virtual StmtTransitiveResult queryBool(const StmtValue &arg0,
                                         const StmtValue &arg1) = 0;
  virtual StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                         const StmtType &type1) = 0;
  virtual StmtTransitiveResult queryTo(const StmtType &type0,
                                       const StmtValue &arg1) = 0;
  virtual StmtTransitiveResult queryAll(const StmtType &type0,
                                        const StmtType &type1) = 0;
};

typedef unique_ptr<ICFGClauseQuerier> ICFGQuerierPtr;
