#pragma once

#include <unordered_set>
#include <memory>

#include "qps/common/IEvaluatable.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/common/VariableTable.h"

using std::unordered_set, std::unique_ptr;

class Clause : public IEvaluatable {
 public:
  virtual bool validateArgTypes(VariableTable* variables) = 0;

 protected:
  static EntityRef toEntityRef(ClauseArgument* arg) {
    return arg->toEntityRef();
  }

  static StmtRef toStmtRef(ClauseArgument* arg) {
    return arg->toStmtRef();
  }
};

typedef unique_ptr<Clause> ClausePtr;
