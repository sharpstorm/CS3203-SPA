#pragma once

#include "ClauseArgument.h"
#include "qps/common/PQLQuerySynonymProxy.h"

class ClauseArgumentFactory {
 public:
  static ClauseArgumentPtr createWildcard();
  static ClauseArgumentPtr create(const PQLQuerySynonymProxy &synProxy);
  static ClauseArgumentPtr create(const StmtValue &stmt);
  static ClauseArgumentPtr create(const EntityValue &ident);
};
